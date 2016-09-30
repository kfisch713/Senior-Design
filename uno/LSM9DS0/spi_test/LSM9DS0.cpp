/**
 *  @file    LSM9DS0.cpp
 *  @author  Michael Linthicum
 *  @date    23 September 2016
 *  @brief   Library for interfacing with LSM9DS0 over SPI.
 */

#include "LSM9DS0.h"

/**
 * Constructor for LSM9DS0 object
 * Sets slave select pin (SC)
 */
LSM9DS0::LSM9DS0(int sc) {
	scPin = sc;
	pinMode(scPin, OUTPUT);
}

/**
 * Reads a register via SPI
 */
byte LSM9DS0::spiRead(byte address) {
	byte data;

	// Enable data transfer
	digitalWrite(scPin, LOW);

	// Send read instruction to register and read it to dest
	SPI.transfer(READ | address);
	data = SPI.transfer(0x00);

	// Disable data transfer
	digitalWrite(scPin, HIGH);

	return data;
}

/**
 * Writes to a register via SPI
 */
void LSM9DS0::spiWrite(byte address, byte data) {

	// Enable data transfer
	digitalWrite(scPin, LOW);

	SPI.transfer(address);
	SPI.transfer(data);

	// Disable data transfer
	digitalWrite(scPin, HIGH);
}

/**
 * Writese to appropriate control registers to allow for all axis acceleration
 * data reading
 */
void LSM9DS0::accelerometerInit() {
	/* CTRL_REG0_XM
	*   7       6       5       4       3       2       1       0
	* BOOT   FIFO_EN WTM_EN     0       0   HP_Click  HPIS1   HPIS2
	* 
	* Set all bits to 0 to begin
	* FIFO will be enabled later 
	*/
	spiWrite(CTRL_REG0_XM, 0b00000000);

	/* CTRL_REG1_XM  
	*   7       6       5       4       3       2       1       0
	* AODR3   AODR2   AODR1   AODR0    BDU    AZEN    AYEN    AXEN
	*
	* Set output data rate (ODR) to 100 Hz
	* Set registers for continuous update
	* Only enable X-axis measurement
	*/ 
	spiWrite(CTRL_REG1_XM, 0b01100111);

	/* CTRL_REG2_XM
	*   7       6       5       4       3       2       1       0
	* ABW1    ABW0    AFS2    AFS1    AFS0    AST1    AST0     SIM
	* 
	* Default anti-alias filter bandwidth (773 Hz)
	* Default acceleration full scale (+-2 g)
	* Normal mode, not self-test
	*/
	spiWrite(CTRL_REG2_XM, 0b00000000);

	/*
	* CTRL_REG3_XM
	*   7       6       5       4       3       2       1       0
	* P1_BOOT P1_TAP P1_INT1 P1_INT2 P1_INTM P1_DRDYA P1_DRDYM P1_EMPTY
	* 
	* Disable all interrupts except DRDYA
	*/
	spiWrite(CTRL_REG3_XM, 0b00000100);
}
