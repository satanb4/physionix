/**
 * @file emg_filter.cpp
 * @brief Source file for the EMGFilter class
 * @author @satanb4
 * @version 1.0
 * @date 2023-04-15
 * This is the source file for the EMGFilter class. It contains the implementation of the base functionality for the filter. 
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fftw3.h>
#include <cstdlib>
#include <stdlib.h>
#include <functional>
#include "emg_filter.h"

// #define DEBUG

/**
 * @brief Construct a new EMG Filter::EMG Filter object
 * @param filter The filter parameters
 * @param sampleRate The sample rate of the EMG data
 * @param windowSize The window size of the EMG data
 * @param filterOrder The filter order of the EMG data
 * @param lowPassCutoff The low pass cutoff of the EMG data
 * @param highPassCutoff The high pass cutoff of the EMG data
 * @param threshold The threshold of the EMG data
 * @param aggregateDataLimit The aggregate data limit of the EMG data
 * @private buffer The buffer of the EMG data
 * @private running The running of the EMG data [bool]
*/
void EMGFilter::set_filter_params(EMG_filter filter)
{
    this->sampleRate = filter.sampleRate;
    this->windowSize = filter.windowSize;
    this->filterOrder = filter.filterOrder;
    this->lowPassCutoff = filter.lowPassCutoff;
    this->highPassCutoff = filter.highPassCutoff;
    this->threshold = filter.threshold;
    this->aggregateDataLimit = filter.aggregateDataLimit;
    buffer = std::vector<double>(windowSize, 0);
    running = false;
}

/**
 * @brief Destroy the EMG Filter::EMG Filter object
 * Passes the running flag to false and joins the thread
*/
void EMGFilter::stop() {
    running = false;
    // Stop the EMG processing thread
    if (emgThread->joinable()) {
        emgThread->join();
        delete emgThread;
        emgThread = nullptr;
    }
}

/// @brief  Start the EMG processing thread
void EMGFilter::start()
{
    if(nullptr!=emgThread) return;
    emgThread = new std::thread(&EMGFilter::start_processing,this);
    
}

/// @brief The main processing loop
void EMGFilter::start_processing() {
    running = true;
    while(running){
            // add data to circular buffer
            buffer.insert(buffer.end(), emgData.begin(), emgData.end());
            buffer.erase(buffer.begin(), buffer.begin() + emgData.size());

            // Generate filter coefficients for a butterworth low-pass filter
            lowPassCoeffs = butterworthLowPassCoeffs(filterOrder, lowPassCutoff, sampleRate);
            highPassCoeffs = butterworthLowPassCoeffs(filterOrder, highPassCutoff, sampleRate);

            // Apply high-pass filter
            std::vector<double> highPassData = filterData(buffer, highPassCoeffs);
            // Apply low-pass filter
            std::vector<double> lowPassData = filterData(highPassData, lowPassCoeffs);

            // calculate FFT
            std::vector<double> fftData = calculateFFT(lowPassData);
            #ifdef DEBUG
            std::cout << "FFT data: ";
            for (int i = 0; i < fftData.size(); i++) {
                std::cout << fftData[i] << " ";
            }
            std::cout << std::endl;
            #endif

            // extract movement
            double movement = extractMovement(fftData, threshold);

            // sleep for a short time to avoid high CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
}

/**
 * @brief Generate filter coefficients for a butterworth low-pass filter
 * @param order The order of the filter
 * @param cutoff The cutoff frequency of the filter
 * @param sampleRate The sample rate of the EMG data
 * @return std::vector<double> The filter coefficients
 * @private a The a coefficients of the filter
 * @private b The b coefficients of the filter
 * @private coeffs The filter coefficients
 * @private theta_c The cutoff frequency in radians
 * @private alpha The alpha coefficient
 * The butterworthFilterCoeffs function generates the filter coefficients for a butterworth low-pass filter using the bilinear transform.
*/
std::vector<double> EMGFilter::butterworthLowPassCoeffs(int order, double cutoff, int sampleRate) {
    double theta_c = 2 * M_PI * cutoff / sampleRate;
    double alpha = sin(theta_c) / (2 * order);
    std::vector<double> a(order + 1, 0);
    std::vector<double> b(order + 1, 0);
    a[0] = 1 + 2 * alpha + 2 * alpha * alpha;
    a[1] = -2 * (1 + alpha) * cos(theta_c);
    a[2] = 1 - 2 * alpha + 2 * alpha * alpha;
    b[0] = alpha * alpha;
    b[1] = 2 * alpha * alpha;
    b[2] = alpha * alpha;
    std::vector<double> coeffs(order * 2 + 1, 0);

    // calculate filter coefficients using bilinear
    // transformation
    for (int i = 0; i < order - 1; i++) {
        coeffs[i] = b[i] / a[0];
        coeffs[i + order + 1] = -a[i] / a[0];
    }

    #ifdef DEBUG
    std::cout << "butterworthLowPassCoeffs: " << coeffs[0] << " " << coeffs[1] << " " << coeffs[2] << " " << coeffs[3] << " " << coeffs[4] << " " << coeffs[5] << " " << coeffs[6] << std::endl;
    #endif

    return coeffs;
}

/**
 * @brief Generate filter coefficients for a butterworth high-pass filter
 * @param order The order of the filter
 * @param cutoff The cutoff frequency of the filter
 * @param sampleRate The sample rate of the EMG data
 * @return std::vector<double> The filter coefficients
 * @private a The a coefficients of the filter
 * @private b The b coefficients of the filter
 * @private coeffs The filter coefficients
 * @private theta_c The cutoff frequency in radians
 * @private alpha The alpha coefficient
 * This function generates the filter coefficients for a butterworth high-pass filter using the bilinear transform and the low-pass filter coefficients.
*/
std::vector<double> EMGFilter::butterworthHighPassCoeffs(int order, double cutoff, int sampleRate) {
    double theta_c = 2 * M_PI * sampleRate / cutoff;
    double alpha = sin(theta_c) / (2 * order);
    std::vector<double> a(order + 1, 0);
    std::vector<double> b(order + 1, 0);
    a[0] = 1 + 2 * alpha + 2 * alpha * alpha;
    a[1] = -2 * (1 + alpha) * cos(theta_c);
    a[2] = 1 - 2 * alpha + 2 * alpha * alpha;
    b[0] = alpha * alpha;
    b[1] = 2 * alpha * alpha;
    b[2] = alpha * alpha;
    std::vector<double> coeffs(order * 2 + 1, 0);

    // calculate filter coefficients using bilinear
    // transformation
    for (int i = 0; i < order - 1; i++) {
        coeffs[i] = b[i] / a[0];
        coeffs[i + order + 1] = a[i] / a[0];
    }

    #ifdef DEBUG
    std::cout << "butterworthHighPassCoeffs: " << coeffs[0] << " " << coeffs[1] << " " << coeffs[2] << " " << coeffs[3] << " " << coeffs[4] << " " << coeffs[5] << " " << coeffs[6] << std::endl;
    #endif

    return coeffs;
}


/**
 * @brief Calculate the FFT of the given data
 * @param data The data to be filtered
 * @return std::vector<double> The filtered data
 * @private n The size of the data
 * @private in The input array for FFTW
 * @private out The output array for FFTW
 * @private plan The FFTW plan
 * This function calculates the FFT of the given data using FFTW3 library.
*/
std::vector<double> EMGFilter::calculateFFT(const std::vector<double>&  data) {
    int n = data.size();

    // Allocate input and output arrays for FFTW
    fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);

    // Copy real-valued data into input array
    for (int i = 0; i < n; i++)
    {
        in[i][0] = data[i];
        in[i][1] = 0.0;
    }

    // Create FFTW plan and execute it
    fftw_plan plan = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // Copy complex-valued output into data array
    std::vector<double> fftData(n, 0);
    for (int i = 0; i < n; i++)
    {
        fftData[i] = out[i][0] * out[i][0] + out[i][1] * out[i][1];
    }

    // Free FFTW resources
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    return fftData;
}

/**
 * @brief Filter the given data using the given filter coefficients
 * @param data The data to be filtered
 * @param coeffs The filter coefficients
 * @return std::vector<double> The filtered data
 * @private filteredData The filtered data
 * This function filters the given data using the given filter coefficients.
*/
std::vector<double> EMGFilter::filterData(const std::vector<double>& data, const std::vector<double>& coeffs) {
    std::vector<double> filteredData(data.size(), 0);
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < coeffs.size(); j++) {
            if (i - j >= 0) {
                filteredData[i] += data[i - j] * coeffs[j];
            }
        }
    }

    #ifdef DEBUG
    std::cout << "filterData: " << filteredData[0] << " " << filteredData[1] << " " << filteredData[2] << " " << filteredData[3] << " " << filteredData[4] << " " << filteredData[5] << " " << filteredData[6] << std::endl;
    #endif
    
    return filteredData;
}

/**
 * @brief Extract the movement from the given FFT data
 * @param fftData The FFT data
 * @param threshold The threshold for movement
 * @return double The movement
 * @private movement The movement
 * This function extracts the movement from the given FFT data.
*/
double EMGFilter::extractMovement(const std::vector<double>& fftData, double threshold) {
    double movement = 0;
    for (int i = 0; i < fftData.size(); i++) {
        if (fftData[i] > threshold) {
            movement += fftData[i];
        }
    }
    movement /= fftData.size();
    movement -= 2*10e4;
    movement = abs(movement);
    //std::cout<<"movement: "<< movement <<std::endl;

    newstate =  deducestate(movement);
    movementData.push_back(newstate);
    if (movementData.size() > aggregateDataLimit){
        newstate = aggregateMovement(movementData);
        movementData.clear();
    }

    if (currentstate != newstate)
        movementdetect(newstate);
    if (newstate == RELAXED)
        movementdetect(RELAXED);
    return movement;
}

/**
 * @brief Aggregate the movement data
 * @param movementData The movement data
 * @return double  The highest frequency element corresponding to the state
 * @private n The size of the data
 * @private maxcount The maximum count
 * @private element_having_max_freq The element having maximum frequency
 * This function aggregates the movement data and figures out the highest frequency element then returns the state deduced from that element.
*/
STATES EMGFilter::aggregateMovement(std::vector<double>& movementData) {
    int n = movementData.size();
    int maxcount = 0;
    int element_having_max_freq;
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (movementData[i] == movementData[j])
                count++;
        }
        if (count > maxcount) {
            maxcount = count;
            element_having_max_freq = movementData[i];
        }
    }
    return deducestate(element_having_max_freq);
}

/**
 * @brief Deduce the state from the given movement
 * @param movement The movement
 * @return double The state
 * @private RELAXED_MAX The maximum value for relaxed state
 * @private RELAXED_MIN The minimum value for relaxed state
 * @private FLEXED_MAX The maximum value for flexed state
 * @private FLEXED_MIN The minimum value for flexed state
 * @private ROTATING_MAX The maximum value for rotating state
 * @private ROTATING_MIN The minimum value for rotating state
 * This function deduces the state from the given movement.
*/
STATES EMGFilter::deducestate(double movement)
{
    if (movement < RELAXED_MAX && movement >= RELAXED_MIN)
        return RELAXED;
    else if (movement < FLEXED_MAX && movement >= FLEXED_MIN)
        return FLEXED;
    else if (movement < ROTATING_MAX && movement >= ROTATING_MIN)
        return ROTATING;
    else {
        return UNKNOWN;
    }
        
}

/// @brief Get the movement
/// @return double The movement
double EMGFilter::getMovement() {
    return movement;
}

