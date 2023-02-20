/**
 *
 * "A simple I2C driver for linux platforms"
 * Originally implemented as part of the Physionix 
 * By Midhun Mathew
 * Original date : 17/02/2023
 *
 * Official project location:
 * https://github.com/midhunjac/physionix
 **/

#include <stdio.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include "I2Cdev.h"

I2Cdev::I2Cdev() 
{

}

/// <summary>
/// Read out a buffer from an I2C slave on bus
/// </summary>
/// <param name="slaveaddress">Chip address of the slave nn the bus</param>
/// <param name="rcvbuf">The buffer to store received data</param>
/// <param name="bytes">Number of bytes received</param>
void I2Cdev::read_buf(uint8_t slaveaddress, uint8_t* rcvbuf, size_t bytes)
{
    int fd = open(I2C_BUS_FILE_PATH, O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
    }
    if (ioctl(fd, I2C_SLAVE, slaveaddress) < 0)
    {
        fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
        close(fd);
    }
    read(fd, rcvbuf, bytes);
    close(fd);
}

/// <summary>
/// Write a buffer to an I2C slave on bus
/// </summary>
/// <param name="slaveaddress">The chip address of the I2C slave</param>
/// <param name="transbuf">The transmit buffer.</param>
/// <param name="bytes">Number of bytes received</param>
void I2Cdev::write_buf(uint8_t slaveaddress, uint8_t* transbuf, size_t bytes) 
{
    int fd = open(I2C_BUS_FILE_PATH, O_RDWR);
    if (fd < 0) 
    {
        fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
    }
    if (ioctl(fd, I2C_SLAVE, slaveaddress) < 0) 
    {
        fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
        close(fd);
    }
    write(fd, transbuf, bytes);
    close(fd);
}



