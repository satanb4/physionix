
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
	ADS1115::start(device);
	getchar();
	ADS1115::stop();
	return 0;

}
