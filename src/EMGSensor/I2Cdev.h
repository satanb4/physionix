#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#define I2C_BUS_FILE_PATH "/dev/i2c-1"

#include <stdlib.h>
/** A general I2C device class */
class I2Cdev {
public:
    /** Constructor */
    I2Cdev();

    /** Write a buffer to aN I2C slave */
    static void write_buf(uint8_t slaveaddress, uint8_t* transbuf, size_t bytes);
    /** Read out a buffer from an I2C slave */
    static void read_buf(uint8_t slaveaddress, uint8_t* rcvbuf, size_t bytes);
};
#endif

