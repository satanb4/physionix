#ifndef _I2CDEV_H_
#define _I2CDEV_H_
#include <stdlib.h>

#define I2C_BUS_DEV_FILE_PATH "/dev/i2c-1"

class I2Cdev {
public:
    I2Cdev();

    static void write_std(uint8_t slaveAddress, uint8_t* txdataBuf, size_t number_of_bytes);
    static void read_std(uint8_t slaveAddress, uint8_t* rxdataBuf, size_t number_of_bytes);
};
#endif

