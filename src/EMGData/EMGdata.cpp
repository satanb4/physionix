
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include "EMGdata.h"
#include <cstdlib>


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

void EMGdata::_start()
{
	// daqthread = std::thread(&EMGdata::startDAQ,this);
	// std::system("./PhysionixServer");
	startDAQ();

}
void EMGdata::_stop()
{
	printf("\nExiting app");
	//daqthread.join();
	ADS1115::stop();
	EMGFilter::stop();
}

