#include "ADS1115.h"
#include <stdio.h>
#include <iostream>

#define START 1

int ADS1115::_i2c_write(ADS1115_device device, uint8_t reg, uint16_t data)
{
	int ret = 0;
	int fd = i2cOpen(I2C_BUS, device.addr, 0);
	if (fd < 0)
	{
		std::cout << "Unable to open " << device.addr << "\n";
		return ERR_NOT_FOUND;
	}
	char buf[] = { (char)((data & 0xFF00) >> 8) , (char)(data & 0xFF) };
	ret = i2cWriteI2CBlockData(fd, reg, buf, 2);
	if (ret < 0)
	{
		std::cout << "Write failed to register " << reg << "\n";
		throw "Write failed";
		return ERR_GEN_FAILURE;
	}
	i2cClose(fd);
	return ret;
}

int ADS1115::_i2c_read(ADS1115_device device, uint8_t reg,uint16_t* data)
{
	int ret = 0;
	int fd = i2cOpen(I2C_BUS, device.addr, 0);
	if (fd < 0)
	{
		std::cout << "Unable to open " << device.addr << "\n";
		return ERR_NOT_FOUND;
	}
	char buf[2];
	ret = i2cReadI2CBlockData(fd, reg, buf, 2);
	if (ret < 0)
	{
		std::cout << "Read failed from register " << reg << "\n";
		throw "Read failed";
		return ERR_GEN_FAILURE;
	}
	i2cClose(fd);
	*data = ((uint16_t)buf[0] << 8) | (uint16_t)buf[1];
	return ret;
}

int ADS1115::_i2c_value(ADS1115_device device, uint16_t* data)
{
	int ret = 0;
	int fd = i2cOpen(I2C_BUS, device.addr, 0);
	if (fd < 0)
	{
		std::cout << "Unable to open " << device.addr << "\n";
		return ERR_NOT_FOUND;
	}
	char buf[2] = { 0 };
	ret = i2cReadI2CBlockData(fd, 0, buf, 2);
	if (ret < 0)
	{
		std::cout << "Read failed from conversion register 0 "<< "\n";
		throw "Read failed";
		return ERR_GEN_FAILURE;
	}
	i2cClose(fd);
	*data = ((uint16_t)buf[0] << 8) | (uint16_t)buf[1];
	return ret;
}


int ADS1115::start(ADS1115_device device)
{
	int cfg = gpioCfgGetInternals();
	cfg |= PI_CFG_NOSIGHANDLER;  // (1<<10)
	gpioCfgSetInternals(cfg);
	int status = gpioInitialise();
	if (status < 0)
	{
		std::cout << "Initialization of GPIO failed " << "\n";
		throw "GPIO initialization failed";
		return ERR_GPIO_INIT_FAILURE;
	}
	_i2c_write(device, Lo_thresh_REG, 0x00);
	_i2c_write(device, Hi_thresh_REG, 0x8000);
	uint16_t config = 0;
	_i2c_read(device, CONFIG_REG, &config);
	config &= ~config;
	config = START << 15 | device.input_channel << ANALOG_INPUT_POS | device.pga << PGA_POS | COMP_POL << 3 | COMP_LAT << 2 | device.data_rate << DATA_RATE_POS;
	_i2c_write(device,CONFIG_REG, config);
	gpioSetMode(ALRT_PIN, PI_INPUT);
	gpioSetISRFuncEx(ALRT_PIN, RISING_EDGE, 1000, interrupt_cb, (void*)this);
	return 0;
}

float ADS1115::_compute_scaler(uint8_t pga)
{
	switch (pga) 
	{
		case PGA_6_144:
			return 6.144f;
		case PGA_4_096:
			return 4.096f;
		case PGA_2_048:
			return 2.048f;
		case PGA_1_024:
			return 1.024f;
		case PGA_0_512:
			return 0.512f;
		case PGA_0_256:
			return 0.256f;
	}
	return 0;
}
void ADS1115::_data_ready()
{
	uint16_t value = 0;
	_i2c_value(device, &value);
	float val = (float)value / (float)0x7fff * _compute_scaler(device.pga) * 2;
	newdata(&val);
}

void ADS1115::stop()
{
	gpioSetISRFuncEx(ALRT_PIN, RISING_EDGE, -1, NULL, (void*)this);

	gpioTerminate();
}
