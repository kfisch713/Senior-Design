 /*
  * Top to bottom
  *        CS_AG   9
  *        MOSI    11 
  * 10 CSM MISO    12
  *        SCLK    13
  *        GND     gnd
  *        VCC     3.3V
  */

#include "LSM9DS0.h"

/* Pin outs. */
int ss_ag = 9;
int ss_m = 10;
int mosi = 11;
int miso = 12;
int sck = 13;

/* Create an instance of the LSM9DS0. */
LSM9DS0 lsm(ss_ag, ss_m, mosi, miso, sck);

void setup() {
  Serial.begin(115200);

  /* Start up SPI and read the status register. */
  lsm.spi_read_byte_ag(WHO_AM_I_XM);
  Serial.println(lsm.spi_read_byte_ag(WHO_AM_I_XM), HEX);

  /* Initialize accelerometer control registers. */
  lsm.init_accelerometer();

  /* Initialize gyroscope control registers. */
  lsm.init_gyroscope();

  /* Initialize magnetometer control registers. */
  lsm.init_magnetometer();
}

void loop() {
  //lsm.print_calculated_accelerometer();
  //lsm.print_calculated_gyroscope();
  lsm.print_calculated_magnetometer();
}

