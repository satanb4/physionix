#include <iostream>
#include <vector>
#include <thread>

class EMGFilter {
private:
    const int sampleRate = 200; // sample rate in Hz
    const int windowSize = 256; // window size for FFT
    const int filterOrder = 2; // filter order
    const double lowPassCutoff = 20; // low pass filter cutoff frequency in Hz
    const double highPassCutoff = 20; // high pass filter cutoff frequency in Hz
    const double threshold = 0.1; // movement detection threshold
    vector<double> buffer; // circular buffer for filtering
    std::vector<double> lowPassCoeffs; // low pass filter coefficients
    vector<double> highPassCoeffs; // high pass filter coefficients
    bool running; // flag to indicate if thread is running
    thread emgThread; // thread to run EMG acquisition and filtering
