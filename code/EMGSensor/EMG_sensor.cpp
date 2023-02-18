
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "EMG_sensor.h"

using namespace EMG_sensor;

EMG_dev::EMG_dev(uint8_t i2c_address = 0x08)
{
	_slaveAddress = 0x08;
}

EMG_dev::~EMG_dev() 
{

}

int16_t EMG_dev::readChannel(uint8_t channel_number) {

	uint8_t reg[1] = {0x20};
	i2c.write_std(_slaveAddress, reg, 1);
	i2c.read_std(_slaveAddress, _data, 2);
	uint8_t tmp_hold = _data[1] << 8 | _data[0];
	return tmp_hold;
}