
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include "EMGdata.h"


int EMGdata::_start()
{
	ADS1115_device device;
	device.addr = 0x48;
	device.data_rate = 128;
	device.pga = PGA_2_048;

	ADS1115::start(device);
	EMGFilter::start();
	return 0;

}

