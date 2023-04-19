#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <thread>
#include "Sensor.h"

class Server {
public:
    Server(SENSORfastcgicallback& sensor) {
        m_sensor = sensor;
    };
    void start();
    void stop();

private:
    SENSORfastcgicallback& m_sensor;
    std::thread m_serverThread;
    bool mainRunning{true};
    void setHUPHandler();
};

#endif // SERVER_H