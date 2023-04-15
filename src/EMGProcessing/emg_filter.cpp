#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <fftw3.h>
#include "emg_filter.h"

EMGFilter::EMGFilter(int sampleRate, int windowSize, int filterOrder, double lowPassCutoff, double highPassCutoff, double threshold) {
    this->sampleRate = sampleRate;
    this->windowSize = windowSize;
    this->filterOrder = filterOrder;
    this->lowPassCutoff = lowPassCutoff;
    this->highPassCutoff = highPassCutoff;
    this->threshold = threshold;
    buffer = std::vector<double>(windowSize, 0);
    lowPassCoeffs = butterworthLowPassCoeffs(filterOrder, lowPassCutoff, sampleRate);
    running = false;
}

// Set the data to be processed
void EMGFilter::setData(const std::vector<double>& data) {
    std::unique_lock<std::mutex> lock(dataMutex);
    emgData = data;
    dataReady = true;
    dataCond.notify_one();
}

// Start the EMG processing thread
void EMGFilter::start(std::vector<double>& emgData) {
    running = true;
    EMGFilter::setData(emgData);
    emgThread = std::thread([this, &emgData] {
        while (running) {
            // add data to circular buffer
            buffer.insert(buffer.end(), emgData.begin(), emgData.end());
            buffer.erase(buffer.begin(), buffer.begin() + emgData.size());

            // apply high-pass filter
            std::vector<double> highPassData = filterData(buffer, highPassCoeffs);

            // apply low-pass filter
            std::vector<double> lowPassData = filterData(highPassData, lowPassCoeffs);

            // calculate FFT
            std::vector<double> fftData = calculateFFT(lowPassData);

            // extract movement
            double movement = extractMovement(fftData, threshold);

            // do something with movement data
            // ...

            // sleep for a short time to avoid high CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
}

// Stop the EMG processing thread
void EMGFilter::stop() {
    running = false;
    if (emgThread.joinable()) {
        emgThread.join();
    }
}

// Generate filter coefficients for a butterworth low-pass filter
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
    for (int i = 0; i < order + 1; i++) {
        coeffs[i] = b[i] / a[0];
        coeffs[i + order + 1] = -a[i] / a[0];
    }
    return coeffs;
}

// FFT function
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

// Filter data using IIR filter
std::vector<double> EMGFilter::filterData(const std::vector<double>& data, const std::vector<double>& coeffs) {
    std::vector<double> filteredData(data.size(), 0);
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < coeffs.size(); j++) {
            if (i - j >= 0) {
                filteredData[i] += data[i - j] * coeffs[j];
            }
        }
    }
    return filteredData;
}

// Extract movement from FFT data
double EMGFilter::extractMovement(const std::vector<double>& fftData, double threshold) {
    double movement = 0;
    for (int i = 0; i < fftData.size(); i++) {
        if (fftData[i] > threshold) {
            movement += fftData[i];
        }
    }
    return movement;
}

double EMGFilter::getMovement() {
    return movement;
}

