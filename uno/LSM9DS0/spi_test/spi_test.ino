/*
 * LSM9DS0 Control
 * 
 * Breakout Pins: https://learn.sparkfun.com/tutorials/lsm9ds0-hookup-guide/breakout-overview
 * LSM9DS0 Datasheet: http://www.st.com/content/ccc/resource/technical/document/datasheet/ab/2a/3b/45/f0/92/41/73/DM00087365.pdf/files/DM00087365.pdf/jcr:content/translations/en.DM00087365.pdf
 * Arduino SPI Reference: https://www.arduino.cc/en/Reference/SPI
 * 
 * Arduino  Breakout  SPI    Datasheet  English
 * ---------------------------------------------------------
 * 10       CSXM      SS     CS         Slave Select
 * 11       SDA       MOSI   SDI        Master Out Slave In
 * 12       SDOXM     MISO   SDO        Master In Slave Out
 * 13       SCL       SCK    SPC        Clock
 * 
 * 23 September 2016
 * Michael Linthicum
 */

#include "LSM9DS0.h"

const int sc = 10;
const SPISettings SPISetting = SPISettings(1000000, MSBFIRST, SPI_MODE1);

// Create an instance of the LSM9DS0
LSM9DS0 lsm(sc);

byte xData8;
byte yData8;
byte zData8;

int xData16 = 0;
int yData16 = 0;
int zData16 = 0;

void setup() {
  Serial.begin(9600);

  SPI.begin();
  delay(100);

  // Set up control registers
  lsm.accelerometerInit();

  // Enable accelerometer stream and set watermark to 32 samples
  lsm.spiWrite(FIFO_CTRL_REG, 0b00111111);

  delay(1000);
}

void loop() {
  xData16 = 0;
  yData16 = 0;
  zData16 = 0;
  
  xData16 = lsm.spiRead(OUT_X_H_A);
  xData8 = lsm.spiRead(OUT_X_L_A);
  xData16 << 8;
  xData16 |= xData8;

  yData16 = lsm.spiRead(OUT_Y_H_A);
  yData8 = lsm.spiRead(OUT_Y_L_A);
  yData16 << 8;
  yData16 |= yData8;

  zData16 = lsm.spiRead(OUT_Z_H_A);
  zData8 = lsm.spiRead(OUT_Z_L_A);
  zData16 << 8;
  zData16 |= zData8;
  
  Serial.print("X: ");
  Serial.print(xData16);
  Serial.print(" Y: ");
  Serial.print(yData16);
  Serial.print(" Z: ");
  Serial.print(zData16);
  Serial.print('\n');
  
  delay(100);
}
