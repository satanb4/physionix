
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include "EMGdata.h"


int EMGdata::_start()
{
	EMGFilter filter(200, 256, 2, 20.0, 20.0, 0.1);
	ADS1115_device device;
	device.addr = 0x48;
	device.data_rate = 128;
	device.pga = PGA_2_048;

	EMG_filter filter;
	
	addsamples();
	ADS1115::start(device);
	
	return 0;

}

void EMGdata::addsamples()
{
	for (int i = 0; i < 1000; i++)
	{
		std::vect
	}
}
