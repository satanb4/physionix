#pragma once
#ifndef SENSORFASTCGICALLBACK_H
#define SENSORFASTCGICALLBACK_H

#include <deque>
#include <chrono>
#include "EMGSensor.h"

class SENSORfastcgicallback : public SensorCallback {
public:
    std::deque<float> emgBuffer;
    std::deque<long> timeBuffer;
    long t;
    const int maxBufSize = 50;
    float lastValue = 0;

    void hasSample(float v);
    void forceemg_data(float temp);

private:
    static unsigned long getTimeMS();
};

#endif // SENSORFASTCGICALLBACK_H
