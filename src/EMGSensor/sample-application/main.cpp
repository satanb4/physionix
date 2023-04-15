#include "../ADS1115.h"
#include <stdlib.h>
#include <iostream>

class ADS1115_test : public ADS1115
{
	virtual void newdata(float *data) { printf("\ndata is %f",*data);}
};

int main(int argc, char* argv[]) {
	std::cout << "Starting application";
	ADS1115_test ads1115tester;
	ADS1115_device device;
	device.addr = 0x48;
	device.data_rate = DATA_RATE_128;
	ads1115tester.start(device);
	getchar();
	ads1115tester.stop();
	return 0;
}
