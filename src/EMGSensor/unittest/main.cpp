#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "../include/EMG_sensor.h"

using namespace EMG_sensor;

int main(){
	
	EMG_sensor::EMG_dev testsensor(0x08);
	
	while(1){
		
		for(int i=0; i<8; i++)
		{

			std::cout << "EMG RAW DATA FOR CHANNEL " << i << " is " << testsensor.readChannel(EMG_dev::RAW_DATA,i) << std::endl;
		}

		std::cout << std::endl;
		sleep(1);
		for(int j=0;j<8;j++)
		{
			std::cout << "EMG MILLIVOLTS FOR CHANNEL " << j << " is " << testsensor.readChannel(EMG_dev::MILLIVOLTS,j) << std::endl;
		}
		std::cout << std::endl;
		sleep(3);

	}

}


