#ifndef __ADS1115_H
#define __ADS1115_H

#include <pigpio.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

//error table
#define ERR_NOT_FOUND			-1
#define ERR_NOT_READY			-2
#define ERR_GEN_FAILURE			-3
#define ERR_GPIO_INIT_FAILURE	-4

#define I2C_BUS				1

#define ALRT_PIN			27 //gpio27 
#define AD1115_ADDR			0x48
#define COMP_POL			1	//active high for alert pin. page 29 in datasheet
#define COMP_LAT			1	//ALRT pin latch until cleared. page 29 in datasheet

//Datarates.page 28 in datasheet
#define DATA_RATE_8			0
#define DATA_RATE_16		1
#define DATA_RATE_32		2
#define DATA_RATE_64		3
#define DATA_RATE_128		4	/*default*/
#define DATA_RATE_250		5
#define DATA_RATE_475		6
#define DATA_RATE_860		7


//analog input channels. page 28 in datasheet
#define ANALOG_INPUT0		4
#define ANALOG_INPUT1		5
#define ANALOG_INPUT2		6
#define ANALOG_INPUT3		7

//Programmable gain Amplifier. page 28 in datasheet
#define PGA_6_144			0	/*Not suitable*/
#define PGA_4_096			1	/*Not suitable*/
#define PGA_2_048			2	/*default*/
#define PGA_1_024			3 
#define PGA_0_512			4 
#define PGA_0_256			5

#define DATA_RATE_POS		5
#define PGA_POS				9
#define ANALOG_INPUT_POS	12

struct ADS1115_device
{
	uint8_t addr = AD1115_ADDR;
	uint8_t data_rate = DATA_RATE_128;
	uint8_t pga = PGA_2_048;
	uint8_t input_channel = ANALOG_INPUT0;
};

class ADS1115
{
public:
	int start(ADS1115_device);
	void stop();
	~ADS1115() { stop(); }
	// class should implement this function. This is the callback in case of new events.
	virtual void newdata(float* data) = 0;
private:
	ADS1115_device device;
	void _data_ready();
	static void interrupt_cb(int, int, uint32_t, void* userdata)
	{
		((ADS1115*)userdata)->_data_ready();
	}
	
	//register definitions
	uint8_t CONVERSION_REG = 0;
	uint8_t CONFIG_REG = 1;
	uint8_t Lo_thresh_REG = 2;
	uint8_t Hi_thresh_REG = 3;

	int _i2c_write(ADS1115_device device, uint8_t reg, uint16_t data);
	int _i2c_read(ADS1115_device device, uint8_t reg, uint16_t* data);
	int _i2c_value(ADS1115_device device, uint16_t* data);
	float _compute_scaler(uint8_t pga);
};


#endif
