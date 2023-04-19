/**
 * @file EMGdata.cpp
 * @brief The implementation file for the EMGdata class
 * @author @midhunjac
 * @version 1.0
 * @date 2023-04-14
 * @paragraph This is the implementation file for the EMGdata class. It contains the implementation of the start and stop functions.
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include "EMGdata.h"
#include <cstdlib>

/**
 * @brief The constructor for the EMGdata class
 * @param None
 * @return None
 * @paragraph This is the constructor for the EMGdata class. It initializes the variables. Also declaring a thread for the data acquisition.
 * @bug No known bugs.
*/
void EMGdata::startDAQ()
{
    ADS1115_device device;
    device.addr = 0x48;
    device.data_rate = 128;
    device.pga = PGA_2_048;
    EMG_filter filter;
    filter.filterOrder = 2;
    filter.highPassCutoff =20;
    filter.lowPassCutoff = 20;
    filter.threshold = 0.1;
    filter.sampleRate = 200;
    filter.windowSize = 256;

    ADS1115::start(device);
    EMGFilter::set_filter_params(filter);
    EMGFilter::start();
}

/// @brief Starts the web application thread and the data acquisition thread
void EMGdata::_start()
{
    printf("\nStarting app");
    startDAQ();
    startEmgApi();
}

/// @brief Stops the web application thread and the data acquisition thread
void EMGdata::_stop()
{
    printf("\nExiting app");
    stopEmgApi();
    ADS1115::stop();
    EMGFilter::stop();
}

/// @brief The function to callback the data to the web application
void EMGdata::startEmgApi()
{
    sensor = new Sensor();
    sensor->setCallback(EMGdata::sensorCallback);
    server.setSensor(sensor);
    server.start();
}

void EMGdata::newdata(float* data)
{
    sensor->hasSample(*data);
    printf("\nnew data is %f", *data);
    measuredata.push_back(*data);
    if (measuredata.size() >= 256)
    {
        emgData = measuredata;
        measuredata.clear();
    }
}

void EMGdata::stopEmgApi()
{
    server.stop();
}

void EMGdata::sensorCallback(float* data) {
    EMGdata* instance = reinterpret_cast<EMGdata*>(Server::getInstance()->getSensor()->getCallbackInstance());
    instance->newdata(data);
}
