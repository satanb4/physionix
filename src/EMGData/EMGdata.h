#include "../EMGProcessing/emg_filter.h"
#include "../EMGSensor/ADS1115.h"
#include "../EMGApi/EMGSensor.h"

#ifndef EMGDATA_H

class EMGdata : public ADS1115, public EMGFilter, public SensorCallback
{
private:
    std::vector<double> measuredata;
    SensorCallback scb;
    virtual void newdata(float* data)
    {
        scb.hasSample(*data);
#ifdef DEBUG
        printf("\new data is %f", *data+23);
#endif // DEBUG
        measuredata.push_back(*data);
        if (measuredata.size() >= 256)
        {
            emgData = measuredata;
            measuredata.clear();
        }
    }

    virtual void movementdetect(STATES movement)
    {
        //printf("\nmovement is %d",movement);
    }
    void startDAQ();
    std::thread daqthread;
    void startEmgApi();
    void stopEmgApi();

public:
    EMGdata() {};
    ~EMGdata() {}
	std::vector<double> getData() { return emgData; };
    void _start();
    void _stop();
};


#endif // EMGDATA_H
