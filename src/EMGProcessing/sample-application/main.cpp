/**
 * @file main.cpp
 * @brief Main file for the EMGFIlter application
 * @author @satanb4
 * @version 1.0
 * @date 2023-04-15
 * This is a sample application for the EMGFilter class. It generates random data and passes it to the filter.
*/

#include "../emg_filter.h"
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <vector>

std::vector<double> testdata;
double data;

/// @brief The class to test the EMGFilter class
class filtertest : public EMGFilter
{
	virtual void movementdetect(STATES currstate)
	{
		std::cout << "movement detected" << std::endl;
		std::cout << "state is " << currstate << std::endl;
	}
};
filtertest emgfilter;

/// @brief The function to generate random data
void generatedata()
{
	while (true)
	{
		data = (rand() % 100 / 15)+26;
		// Sleep the thread for 5 ms
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		testdata.push_back(data);
		if(testdata.size() >=256)
		{
			//std::vector<double> data(testdata);
			emgfilter.emgData = testdata;
			testdata.clear();

		}
	}

}

/// @brief The main function setting the filter parameters and calling the filter
/// @param argc The number of arguments
/// @param argv The arguments
/// @return The return value (default 0)
int main(int argc, char* argv[]) {
	std::cout << "Starting Application" << std::endl;
	EMG_filter filter;
	filter.filterOrder = 2;
	filter.highPassCutoff = 600;
	filter.lowPassCutoff = 20;
	filter.threshold = 1500;
	filter.sampleRate = 200;
	filter.windowSize = 256;
	emgfilter.set_filter_params(filter);
	emgfilter.start();
	generatedata();
	getchar();
	emgfilter.stop();
	return 0;

}
