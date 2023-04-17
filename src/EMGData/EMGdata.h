#include "../EMGProcessing/emg_filter.h"
#include "../EMGSensor/ADS1115.h"
#include "../EMGApi/EMGSensor.h"

class EMGdata: public ADS1115, public EMGFilter //public SensorCallback
{
	//SensorCallback* sensorcb;/*do not uncomment. will result in crash as of now.need to fix*/
	
	virtual void newdata(float* data)
	{
#ifdef DEBUG
		printf("\new data is %f", *data+23);
#endif // DEBUG
		//sensorcb->hasSample(*data);
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
	~EMGdata() 
	{
		
	}
	void _start();
	void _stop();
	std::vector<double> measuredata;

};
