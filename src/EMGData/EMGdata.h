/**
 * @file EMGdata.h
 * @brief Header file for the EMGdata class
 * @author @midhunjac
 * @version 1.0
 * @date 2023-04-17
 * @paragraph This is the header file for the EMGdata class. It contains the basic functionality for the data acquisition.
 * @bug No known bugs.
*/

#include "../EMGProcessing/emg_filter.h"
#include "../EMGSensor/ADS1115.h"
#include "../EMGApi/EMGSensor.h"
#include "../EMGApi/Server.h"
#include "../EMGApi/Sensor.h"

#ifndef EMGDATA_H
#define EMGDATA_H

/**
 * @brief The EMGdata class
 * @param None
 * @return None
 * @paragraph This is the header class for the EMGdata. It contains basic functionality definitions for the data acquisition.
*/
class EMGdata : public ADS1115, public EMGFilter, public SensorCallback
{
private:
    std::vector<double> measuredata;
    std::vector<double> emgData;
    SensorCallback* sensorCallback = nullptr;

    void newdata(float* data){};

    virtual void movementdetect(STATES movement)
    {
        //invoke the motor actuation code
    }
    void startDAQ();
    std::thread daqthread;
    void startEmgApi();
    void stopEmgApi();

public:
    EMGdata() {};
    ~EMGdata() {}
    void setCallback(SensorCallback* cb) {
		sensorCallback = cb;
        if (nullptr != sensorCallback) {
                        sensorCallback->hasSample(value);
                }
        }
	}
	std::vector<double> getData() { return emgData; };
    void _start();
    void _stop();
};


#endif // EMGDATA_H
