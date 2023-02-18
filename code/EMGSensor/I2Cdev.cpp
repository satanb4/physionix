
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include "I2Cdev.h"


    I2Cdev::I2Cdev() {
}

void I2Cdev::write_std(uint8_t slaveAddress, uint8_t* txdataBuf, size_t number_of_bytes) {
    int fd = open(I2C_BUS_DEV_FILE_PATH, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
    }
    if (ioctl(fd, I2C_SLAVE, slaveAddress) < 0) {
        fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
        close(fd);
    }
    write(fd, txdataBuf, number_of_bytes);
    close(fd);
}

void I2Cdev::read_std(uint8_t slaveAddress, uint8_t* rxdataBuf, size_t number_of_bytes) {
    int fd = open(I2C_BUS_DEV_FILE_PATH, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
    }
    if (ioctl(fd, I2C_SLAVE, slaveAddress) < 0) {
        fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
        close(fd);
    }
    read(fd, rxdataBuf, number_of_bytes);
    close(fd);
}


