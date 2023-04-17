#include "../EMGProcessing/emg_filter.h"
#include "../EMGSensor/ADS1115.h"

#ifndef EMGDATA_H

class EMGdata : public ADS1115, public EMGFilter
{
private:
    std::vector<double> measuredata;

    virtual void newdata(float* data)
    {
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

public:
    EMGdata() {};
    ~EMGdata() {}
	std::vector<double> getData() { return emgData; };
    void _start();
    void _stop();
};


#endif // EMGDATA_H
