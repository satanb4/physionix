#ifndef EMGDATA_H
#define EMGDATA_H

#include <vector>
#include "../EMGSensor/ADS1115.h"
#include "../EMGProcessing/emg_filter.h"
#include "../EMGApi/Server.h"

class EMGdata {
public:
    EMGdata() {}
    ~EMGdata() {}
    void startDAQ();
    void _start();
    void _stop();
    void startEmgApi();
    void stopEmgApi();
    void newdata(float* data);
    static void sensorCallback(float* data);
private:
    Server server;
    std::vector<float> measuredata;
    std::vector<float> emgData;
};

#endif
