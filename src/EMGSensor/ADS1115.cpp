#include "ADS1115.h"
#include <stdio.h>
#include <iostream>

int ADS1115::i2c_write(ADS1115_device device, uint8_t reg, uint16_t data)
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

int ADS1115::i2c_read(ADS1115_device device, uint8_t reg,uint16_t* data)
{
	int ret = 0;
	int fd = i2cOpen(I2C_BUS, device.addr, 0);
	if (fd < 0)
	{
		std::cout << "Unable to open " << device.addr << "\n";
		return ERR_NOT_FOUND;
	}
	char buf[2] = { 0 };
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

int ADS1115::i2c_value(ADS1115_device device, uint16_t* data)
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
	i2c_write(device, Lo_thresh_REG, 0x00);
	i2c_write(device, Hi_thresh_REG, 0x8000);
	uint16_t config = 0;
	i2c_read(device, CONFIG_REG, &config);
	config &= ~config;
	config = 1 << 15 | 1 << 14 | PGA << 9 | 1 << 3 | 1 << 2;
	i2c_write(device,CONFIG_REG, config);
	gpioSetMode(ALRT_PIN, PI_INPUT);
	gpioSetISRFuncEx(ALRT_PIN, RISING_EDGE, 1000, interrupt_cb, (void*)this);
}

void ADS1115::_data_ready()
{
	uint16_t value = 0;
	i2c_value(device, &value);
	newdata((float)value);
}
void ADS1115::main()
{
	std::cout << "Starting application";
}