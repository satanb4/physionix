#include <stdint.h>
#include "I2Cdev.h"
#define I2C_BUS "/dev/i2c-1"
namespace EMG_sensor {
	class EMG_dev {
	private:	
		uint8_t _slaveAddress;
		I2Cdev i2c;
		uint8_t _data[2];

		void _writeConfig();
		void _setChannel(uint8_t channel_number);
	public:
		EMG_dev(uint8_t i2c_address);
		~EMG_dev();
		int16_t readChannel(uint8_t channel_number);
	};
}
