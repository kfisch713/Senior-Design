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
#define OUT_X_L_A       0x28
#define OUT_X_H_A       0x29
#define OUT_Y_L_A       0x2A
#define OUT_Y_H_A       0x2B
#define OUT_Z_L_A       0x2C
#define OUT_Z_H_A       0x2D
#define FIFO_CTRL_REG   0x2E
#define CTRL_REG0_XM    0x1f
#define CTRL_REG1_XM    0x20
#define CTRL_REG2_XM    0x21
#define CTRL_REG3_XM    0x22
#define FIFO_SRC_REG    0x2F

class LSM9DS0{
  public:
    /* Initialize an LSM9DS0 object and set the slave select pin high. */
    LSM9DS0(int ss, int mosi, int miso, int sck);

    /* Transfer a byte to the device and read a byte from the device. */
    byte spi_transfer(byte data);

    /* Reads a register and returns the value. */
    byte spi_read_byte(byte address);

    /* Read a certain number of bytes from a register. */
    void spi_read_bytes(byte address, byte* data, byte count);
    
    /* Writes a value to a specified register. */
    void spi_write(byte address, byte data);

    /* Reads accelerometer values and prints them raw. */
    void print_accelerometer();

    /* Initialize data transfer registers for accelerometer output. */
    void init_accelerometer();

    /* All possible output data rates of the accelerometer. */
    enum a_odr {
      A_POWER_DOWN, /* Power down mode (0000) */
      A_ODR_3125,   /* 3.125 Hz        (0001) */
      A_ODR_625,    /* 6.25 Hz         (0010) */
      A_ODR_125,    /* 12.5 Hz         (0011) */
      A_ODR_25,     /* 25 Hz           (0100) */
      A_ODR_50,     /* 50 Hz           (0101) */
      A_ODR_100,    /* 100 Hz          (0110) */
      A_ODR_200,    /* 200 Hz          (0111) */
      A_ODR_400,    /* 400 Hz          (1000) */
      A_ODR_800,    /* 800 Hz          (1001) */
      A_ODR_1600    /* 1600 Hz         (1010) */
    };

    /* Set the output data rate for the accelerometer. */
    void init_accelerometer_odr(a_odr rate);

    /* All possible scale values for the accelerometer. */
    enum a_scale {
      A_SCALE_2G,  /* 2G  (000) */
      A_SCALE_4G,  /* 4G  (001) */
      A_SCALE_6G,  /* 6G  (010) */
      A_SCALE_8G,  /* 8G  (011) */
      A_SCALE_16G, /* 16G (100) */
    };

    /* Set the scale for the accelerometer. */
    void init_accelerometer_scale(a_scale scale);

  private:
    int SS_PIN;
    int MOSI_PIN;
    int MISO_PIN;
    int SCK_PIN;
};

#endif

