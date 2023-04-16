#include "../emg_filter.h"
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <functional>

std::vector<double> testdata;
double data;
class filtertest : public EMGFilter
{
	virtual void movementdetect(STATES currstate)
	{
		printf("\nmovement is %d", currstate);
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
			std::vector<double> data(testdata);
			testdata.clear();
			emgfilter.start(data);
			//std::thread workthread = std::thread(&filtertest::start,std::ref(data));
		}
		//printf("\n%f",data);
	}

}
int main(int argc, char* argv[]) {
	printf("\nstarting application");
	EMG_filter filter;
	filter.filterOrder = 2;
	filter.highPassCutoff =20;
	filter.lowPassCutoff = 20;
	filter.threshold = 0.1;
	filter.sampleRate = 200;
	filter.windowSize = 256;
	emgfilter.set_filter_params(filter);
	generatedata();
	getchar();
	//emgfilter.stop();
	return 0;

}
