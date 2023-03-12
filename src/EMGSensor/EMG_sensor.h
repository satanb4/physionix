#ifndef _EMGSENSOR_H_
#define _EMGSENSOR_H_

#include <stdint.h>
#include "I2Cdev.h"

#define I2C_BUS "/dev/i2c-1"

namespace EMG_sensor {

	/**
	* Class for the EMG device in use.
	*/
	class EMG_dev {

	private:

		uint8_t _slaveaddress;	/** The slave address of the I2C device on the bus */
		I2Cdev i2c;				/** The I2C bus									   */
		uint8_t _data[2];

		/** TODO if necessary, a function to set to operate only on a specific channel */
		void _setChannel(uint8_t channel_number);

	public:
		/** enum _data_type.
		*	There are two types of readings generated from each channels (1-8)
		*		- Raw data with maximum value 4096
		*		- Reading in millivolts with maximum value 3300mV
		*/
		enum _data_type
		{
			RAW_DATA,
			MILLIVOLTS
		};

		/** Constructor for the class */
		EMG_dev(uint8_t i2c_address);
		
		/** Destructor */
		~EMG_dev();
		
		/** Read out data from a particular channel */
		int16_t readChannel(_data_type type,uint8_t channel_number);

	};
}
#endif

