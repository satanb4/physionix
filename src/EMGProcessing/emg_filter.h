#include <fftw3.h>
#include <iostream>
#include <vector>

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
#define RELAXED_MAX     20
#define FLEXED_MIN      20
#define FLEXED_MAX      300
#define ROTATING_MAX    300
#define ROTATING_MIN    700

enum STATES
{
    RELAXED,
    FLEXED,
    ROTATING
};
class EMGFilter {
public:
    EMGFilter(int sampleRate = 200, int windowSize = 256, int filterOrder = 2, double lowPassCutoff = 20, double highPassCutoff = 20, double threshold = 0.1);
    double getMovement();
    void setData(const std::vector<double>& data);
    void start(std::vector<double>& emgData);
    void stop();
    virtual void movementdetect(STATES movement) = 0;
private:
    int sampleRate;
    int windowSize;
    int filterOrder;
    double lowPassCutoff;
    double highPassCutoff;
    double threshold;
    STATES currentstate = RELAXED;
    STATES newstate;
    std::vector<double> buffer;
    std::vector<double> lowPassCoeffs;
    std::vector<double> highPassCoeffs;
    std::vector<double> emgData;
    double movement;
    bool running;
    bool dataReady;
    std::condition_variable dataCond;
    std::mutex dataMutex;
    std::thread emgThread;

    std::vector<double> butterworthLowPassCoeffs(int order, double cutoff, int sampleRate);
    std::vector<double> filterData(const std::vector<double>& data, const std::vector<double>& coeffs);
    std::vector<double> calculateFFT(const std::vector<double>& data);
    double extractMovement(const std::vector<double>& fftData, double threshold);
    STATES deducestate(double movement);
};

#endif
