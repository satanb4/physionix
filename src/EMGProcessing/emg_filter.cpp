#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <fftw3.h>
#include "include/emg_filter.h"


class EMGFilter {
    private:
        const int sampleRate = 200; // sample rate in Hz
        const int windowSize = 256; // window size for FFT
        const int filterOrder = 2; // filter order
        const double lowPassCutoff = 20; // low pass filter cutoff frequency in Hz
        const double highPassCutoff = 20; // high pass filter cutoff frequency in Hz
        const double threshold = 0.1; // movement detection threshold
        std::vector<double> buffer; // circular buffer for filtering
        std::vector<double> lowPassCoeffs; // low pass filter coefficients
        std::vector<double> highPassCoeffs; // high pass filter coefficients
        bool running; // flag to indicate if thread is running
        std::thread emgThread; // thread to run EMG acquisition and filtering

    public:
        EMGFilter() {
            // initialize circular buffer
            buffer = std::vector<double>(windowSize, 0);

            // initialize filter coefficients
            lowPassCoeffs = butterworthLowPassCoeffs(filterOrder, lowPassCutoff, sampleRate);
            highPassCoeffs = butterworthHighPassCoeffs(filterOrder, highPassCutoff, sampleRate);
        }

        void start(std::vector<double>& emgData) {
            running = true;
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

        void stop() {
            running = false;
            if (emgThread.joinable()) {
                emgThread.join();
            }
        }

    private:
        std::vector<double> butterworthLowPassCoeffs(int order, double cutoff, int sampleRate) {
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
};