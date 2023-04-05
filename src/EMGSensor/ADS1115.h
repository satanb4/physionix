#ifndef __ADS1115RPI_H
#define __ADS1115RPI_H

#include <pigpio.h>
#include <stdint.h>
#include <unistd.h>

//error table
#define ERR_NOT_FOUND -1
#define ERR_NOT_READY -2
#define ERR_GEN_FAILURE -3
#define ERR_GPIO_INIT_FAILURE -4

#define I2C_BUS 1

#define ALRT_PIN 27 //gpio27 board pin 13
#define AD1115_ADDR 0x48
#define DATA_RATE 0
#define PGA 2 //default
struct ADS1115_device
{
	uint8_t addr = AD1115_ADDR;
	uint16_t datarate = DATA_RATE;
	uint16_t pga = PGA;
};

class ADS1115
{
public:
	int start(ADS1115_device);
	int stop();
	~ADS1115() { stop(); }
	virtual void newdata(float data);
private:
	ADS1115_device device;
	void _data_ready();
	static void interrupt_cb(int gpio, int level, uint32_t tick, void* userdata)
	{
		((ADS1115*)userdata)->_data_ready();
	}
	
	//register definitions
	uint8_t CONVERSION_REG = 0;
	uint8_t CONFIG_REG = 1;
	uint8_t Lo_thresh_REG = 2;
	uint8_t Hi_thresh_REG = 3;

	int i2c_write(ADS1115_device device, uint8_t reg, uint16_t data);
	int i2c_read(ADS1115_device device, uint8_t reg, uint16_t* data);
	int i2c_value(ADS1115_device device, uint16_t* data);
	void main();
};


#endif
