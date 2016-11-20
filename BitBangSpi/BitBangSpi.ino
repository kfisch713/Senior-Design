/*
 * LSM9DS0 Control
 * 
 * Arduino  Breakout  SPI    Datasheet  English
 * ---------------------------------------------------------
 * 10       CSXM      SS     CS         Slave Select
 * 11       SDA       MOSI   SDI        Master Out Slave In
 * 12       SDOXM     MISO   SDO        Master In Slave Out
 * 13       SCL       SCK    SPC        Clock
 * 
 */

#include "LSM9DS0.h"

/* Pin outs. */
int ss = 10;
int mosi = 11;
int miso = 12;
int sck = 13;

/* Create an instance of the LSM9DS0. */
LSM9DS0 lsm(ss, mosi, miso, sck);

void setup() {
  Serial.begin(9600);

  /* Start up SPI and read the status register. */
  lsm.spi_read_byte(WHO_AM_I_XM);
  Serial.println(lsm.spi_read_byte(WHO_AM_I_XM), HEX);

  /* Initialize accelerometer control registers. */
  lsm.init_accelerometer();
  lsm.init_accelerometer_odr(lsm.A_ODR_100);
  lsm.init_accelerometer_scale(lsm.A_SCALE_6G);
}

void loop() {
  lsm.print_calculated_accelerometer();
}

