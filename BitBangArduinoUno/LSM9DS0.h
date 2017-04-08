/**
 *  @file    LSM9DS0.h
 *  @author  Michael Linthicum
 *  @date    19 November 2016
 *  @brief   Library for flashing LSM9DS0 compatablilty.
 */

#ifndef LSM9DS0_H
#define LSM9DS0_H

#include "Arduino.h"

#define READ            0x80
#define MULTIPLE_READ   0xC0
#define WHO_AM_I_XM     0x0f
#define FIFO_CTRL_REG   0x2E
#define CTRL_REG1_G     0x10
#define CTRL_REG2_G     0x11
#define CTRL_REG3_G     0x12
#define CTRL_REG4       0x1E
#define ORIENT_CFG_G    0x13
#define CTRL_REG5_XL    0x1F
#define CTRL_REG6_XL    0x20
#define CTRL_REG7_XL    0x21
#define OUT_X_L_XL      0x28
#define OUT_X_L_G       0x18
#define FIFO_SRC_REG    0x2F
#define CTRL_REG1_M     0x20
#define CTRL_REG2_M     0x21
#define CTRL_REG3_M     0x22
#define CTRL_REG4_M     0x23
#define CTRL_REG5_M     0x24
#define OUT_X_L_M       0x28

class LSM9DS0{
  public:
    /* Initialize an LSM9DS0 object and set the slave select pin high. */
    LSM9DS0(int ss_ag, int ss_m, int mosi, int miso, int sck);

    /* Transfer a byte to the device and read a byte from the device. */
    byte spi_transfer(byte data);

    /* Reads a register and returns the value. */
    byte spi_read_byte_ag(byte address);

    /* Reads a register and returns the value. */
    byte spi_read_byte_m(byte address);

    /* Read a certain number of bytes from a register. */
    void spi_read_bytes_ag(byte address, byte* data, byte count);

    /* Read a certain number of bytes from a register. */
    void spi_read_bytes_m(byte address, byte* data, byte count);
    
    /* Writes a value to a specified register. */
    void spi_write_ag(byte address, byte data);

    /* Writes a value to a specified register. */
    void spi_write_m(byte address, byte data);

    /* Reads accelerometer values and prints them raw. */
    void print_raw_accelerometer();

    /* Reads and calculates accelerometer values and prints them. */
    void print_calculated_accelerometer();

    /* Initialize data transfer registers for accelerometer output. */
    void init_accelerometer();

    /* Initialize data transfer registers for gyroscope output. */
    void init_gyroscope();

    /* Reads gyroscope values and prints them raw. */
    void print_raw_gyroscope();

    /* Reads and calculates gyroscope values and prints them. */
    void print_calculated_gyroscope();
    
    /* Initialize data transfer registers for magnetometer output. */
    void init_magnetometer();
    
    /* Reads magnetometer values and prints them raw. */
    void print_raw_magnetometer();

    /* Reads and calculates magnetometer values and prints them. */
    void print_calculated_magnetometer();

  private:
    int SS_AG_PIN;
    int SS_M_PIN;
    int MOSI_PIN;
    int MISO_PIN;
    int SCK_PIN;
};

#endif

