/**
 * @file emg_filter.h
 * @brief Header file for the EMGFilter class
 * @author @satanb4
 * @version 1.0
 * @date 2023-04-15
 * This is the header file for the EMGFilter class. It contains the basic functionality for the filter. 
*/

#include <fftw3.h>
#include <iostream>
#include <vector>
#include <thread>

#ifndef EMG_FILTER_H
#define EMG_FILTER_H

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>

#define RELAXED_MIN     0
#define RELAXED_MAX     3000
#define FLEXED_MIN      9000
#define FLEXED_MAX      12000
#define ROTATING_MAX    3000
#define ROTATING_MIN    9000

#define DEFAULT_SAMPLERATE          200
#define DEFAULT_WINDOWSIZE          256
#define DEFAULT_FILTERORDER         2
#define DEFAULT_LOWPASSCUTOFF       20
#define DEFAULT_HIGHPASSCUTOFF      600
#define DEFAULT_THRESHOLD           20
#define DEFAULT_MOVEMENTDATA_SIZE   5000

/// @brief The states of the muscle
enum STATES
{
    RELAXED,
    FLEXED,
    ROTATING,
    UNKNOWN
};

/// @brief The structure to hold the filter parameters
struct EMG_filter
{
    int     sampleRate      =       DEFAULT_SAMPLERATE;
    int     windowSize      =       DEFAULT_WINDOWSIZE;
    int     filterOrder     =       DEFAULT_FILTERORDER;
    double  lowPassCutoff   =       DEFAULT_LOWPASSCUTOFF;
    double  highPassCutoff  =       DEFAULT_HIGHPASSCUTOFF;
    double  threshold       =       DEFAULT_THRESHOLD;
    int     aggregateDataLimit =      DEFAULT_MOVEMENTDATA_SIZE;

};

/**
 * @brief The EMGFilter class
 * This class is the header for the base class of the EMG filter. It contains the basic functionality for the filter.
 * The class is abstract and the derived class should implement the movementdetect function.
*/
class EMGFilter {
public:
    EMGFilter() {}
    std::vector<double> emgData;

    void set_filter_params(EMG_filter);
    double getMovement();
    void start();
    void stop();
    virtual void movementdetect(STATES movement) = 0;

private:
    int sampleRate;
    int windowSize;
    int filterOrder;
    int aggregateDataLimit;
    double lowPassCutoff;
    double highPassCutoff;
    double threshold;
    STATES currentstate = RELAXED;
    STATES newstate;
    std::vector<double> buffer;
    std::vector<double> lowPassCoeffs;
    std::vector<double> highPassCoeffs;
    std::vector<double> movementData;
    double movement;
    bool running;
    bool dataReady;
    std::condition_variable dataCond;
    std::mutex dataMutex;
    std::thread* emgThread = nullptr;
    void start_processing();
    std::vector<double> butterworthLowPassCoeffs(int order, double cutoff, int sampleRate);
    std::vector<double> butterworthHighPassCoeffs(int order, double cutoff, int sampleRate);
    std::vector<double> filterData(const std::vector<double>& data, const std::vector<double>& coeffs);
    std::vector<double> calculateFFT(const std::vector<double>& data);
    double extractMovement(const std::vector<double>& fftData, double threshold);
    STATES aggregateMovement(std::vector<double>& movementData);
    STATES deducestate(double movement);
};

#endif
