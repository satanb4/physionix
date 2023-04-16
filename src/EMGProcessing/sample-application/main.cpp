#include "../emg_filter.h"
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <vector>

std::vector<double> testdata;
double data;
class filtertest : public EMGFilter
{
	virtual void movementdetect(STATES currstate)
	{
		std::cout << "movement detected" << std::endl;
		std::cout << "state is " << currstate << std::endl;
	}
};
filtertest emgfilter;

void generatedata()
{
	while (true)
	{
		 data = (rand() % 100 / 15)+26;
		testdata.push_back(data);
		if(testdata.size() >=256)
		{
			//std::vector<double> data(testdata);
			emgfilter.emgData = testdata;
			testdata.clear();

		}
	}

}
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
