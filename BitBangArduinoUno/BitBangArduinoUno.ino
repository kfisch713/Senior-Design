/*
 * LSM9DS0 Control
 * 
 * Arduino  Breakout  SPI    Datasheet  English
 * ---------------------------------------------------------
 * 9        CSAG      SS     CS         Slave Select
 * 11       SDA       MOSI   SDI        Master Out Slave In
 * 12       SDOXM     MISO   SDO        Master In Slave Out
 * 13       SCL       SCK    SPC        Clock
 * 
 */

#include "LSM9DS0.h"

/* Pin outs. */
int ss = 9;
int mosi = 11;
int miso = 12;
int sck = 13;

/* Create an instance of the LSM9DS0. */
LSM9DS0 lsm(ss, mosi, miso, sck);

void setup() {
  Serial.begin(115200);

  /* Start up SPI and read the status register. */
  lsm.spi_read_byte(WHO_AM_I_XM);
  Serial.println(lsm.spi_read_byte(WHO_AM_I_XM), HEX);

  /* Initialize accelerometer control registers. */
  lsm.init_accelerometer();
}

void loop() {
  lsm.print_raw_accelerometer();
}

