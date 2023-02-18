/*
Adapted from https://github.com/Seeed-Studio/grove.py for C++ as part of the Physionix project
Original Licence:
The MIT License (MIT)
Grove 8 channels 12 bit ADC Hat for the Raspberry Pi, used to connect grove sensors.
Copyright (C) 2018  Seeed Technology Co.,Ltd.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
*/

#include "EMG_sensor.h"
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

using namespace EMG_sensor;

EMG_dev::EMG_dev(uint8_t i2c_address = 0x08)
{
	_slaveaddress = 0x08;
}

EMG_dev::~EMG_dev() 
{

}


/// <summary>
/// Read data from a particular channel (1-8) of the ADC
/// </summary>
/// <param name="type">The mode of reading - either raw data or reading in millivolts</param>
/// <param name="channel_number">Channel number (1-8) to read from</param>
/// <returns>Read value from the I2C device</returns>
int16_t EMG_dev::readChannel(_data_type type,uint8_t channel_number) 
{

	uint8_t reg[1] = {0x10};
	(type == RAW_DATA) ? reg[0] = 0x10+channel_number: reg[0] = 0x20+channel_number;
	i2c.write_buf(_slaveaddress, reg, 1);
	i2c.read_buf(_slaveaddress, _data, 2);
	uint8_t tmp_hold = _data[1] << 8 | _data[0];
	return tmp_hold;
}
