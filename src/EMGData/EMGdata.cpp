
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include "EMGdata.h"
#include "../EMGApi/EMGSensor.h"
#include <cstdlib>

bool mainRunning = false;

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
	printf("\nStarting app");
	
	startEmgApi(mainRunning);
	// Start the data acquisition in the background
	daqthread = std::thread(&EMGdata::startDAQ,this);
	std::system("./PhysionixServer");
	startDAQ();

}
void EMGdata::_stop()
{
	printf("\nExiting app");
	stopEmgApi(mainRunning);
	daqthread.join();
	ADS1115::stop();
	EMGFilter::stop();
}

void startEmgApi(mainrunning) {
	EMGdata emgdata;

	SENSORfastcgicallback sensorfastcgicallback;
	// Create the sensor
	EMGSensor sensor;
	SENSORPOSTCallback postCallback(&sensorfastcgicallback);
	// Set the callback
	sensor.setCallback(&postCallback);
	JSONCGIADCCallback fastCGIADCCallback(&sensorfastcgicallback);

	// creating an instance of the fast CGI handler
	JSONCGIHandler jsoncgiHandler;

	// starting the fastCGI handler with the callback and the
	// socket for nginx.
	jsoncgiHandler.start(&fastCGIADCCallback,&postCallback,
							    "/tmp/sensorsocket");

	// catching Ctrl-C or kill -HUP so that we can terminate properly
	setHUPHandler();
	while (mainRunning) sleep(1);
	
}

void stopEmgApi(mainrunning) {
	if (mainRunning) {
		mainRunning = false;
		jsoncgiHandler.stop();
	}
}

