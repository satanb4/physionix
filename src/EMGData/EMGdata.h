#include "../EMGProcessing/emg_filter.h"
#include "../EMGSensor/ADS1115.h"

class EMGdata: public ADS1115, public EMGFilter
{
	EMGdata() {};
	~EMGdata() 
	{
		EMGFilter::stop();
		ADS1115::stop();
	}
	virtual void newdata(float* data)
	{
#ifdef DEBUG
		printf("\new data is %f", *data);
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
		printf("\nmovement is %d",movement);
	}
public:
	int _start();
	void addsamples();
	std::vector<double> measuredata;


};