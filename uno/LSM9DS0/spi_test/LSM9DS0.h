/**
 *  @file    LSM9DS0.h
 *  @author  Michael Linthicum
 *  @date    23 September 2016
 *  @brief	 Library for flashing LSM9DS0 compatablilty.
 */

#ifndef LSM9DS0_H
#define LSM9DS0_H

#include "Arduino.h"
#include "SPI.h"

#define READ            0x80
#define WHO_AM_I_XM     0x0f
#define OUT_X_L_A       0x28
#define FIFO_CTRL_REG   0x2E
#define CTRL_REG0_XM    0x1f
#define CTRL_REG1_XM    0x20
#define CTRL_REG2_XM    0x21
#define CTRL_REG3_XM    0x22

class LSM9DS0{
	public:
		LSM9DS0(int sc);
		byte spiRead(byte address);
		void spiWrite(byte address, byte data);
		void accelerometerInit();
	private:
		int scPin;
};

#endif