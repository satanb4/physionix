#include "../EMGProcessing/emg_filter.h"
#include "../EMGSensor/ADS1115.h"

class EMGdata: public ADS1115, public EMGFilter
{
	virtual void newdata(float* data)
	{
		printf("\new data is %f", *data);
	}
public:
	int _start();
	
	

};