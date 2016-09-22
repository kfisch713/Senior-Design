/*
 * LSM9DS0 Control
 * 
 * Breakout Pins: https://learn.sparkfun.com/tutorials/lsm9ds0-hookup-guide/breakout-overview
 * LSM9DS0 Datasheet: http://www.st.com/content/ccc/resource/technical/document/datasheet/ab/2a/3b/45/f0/92/41/73/DM00087365.pdf/files/DM00087365.pdf/jcr:content/translations/en.DM00087365.pdf
 * Arduino SPI Reference: https://www.arduino.cc/en/Reference/SPI
 * 
 * Pin configurations and all the names because nothing is easy..
 * Arduino  Breakout  SPI    Datasheet  English
 * 10       CSXM      SS     CS         Slave Select
 * 11       SDA       MOSI   SDI        Master Out Slave In
 * 12       SDOXM     MISO   SDO        Master In Slave Out
 * 13       SCL       SCK    SPC        Clock
 * 
 * 20 September 2016
 * Michael Linthicum
 */

#include<SPI.h>

// Set pin 10 as the slave select (SS)
const int slaveSelectPin = 10;
const SPISettings SPISetting = SPISettings(1000000, MSBFIRST, SPI_MODE1);

const char READ          = 0x80;
const char WHO_AM_I_XM   = 0x0f;
const char OUT_X_L_A     = 0x28;
const char FIFO_CTRL_REG = 0x2E;
const char CTRL_REG0_XM  = 0x1f;
const char CTRL_REG1_XM  = 0x20;
const char CTRL_REG2_XM  = 0x21;
const char CTRL_REG3_XM  = 0x22;

void setup() {
  Serial.begin(115200);
  pinMode(slaveSelectPin, OUTPUT);
  
  // Start SPI Library functionality
  SPI.begin();
  
  // Let the chip do its startup nonsense
  delay(100);

  // Get register data from accelerometer whoami
  spiRead(WHO_AM_I_XM);

  // Enable accelerometer fifo
  char c = spiRead(CTRL_REG0_XM);
  spiWrite(CTRL_REG0_XM, c | 0x40);
  delay(100);

  accelerometerInit();

  // Enable accelerometer stream and set watermark to 32 samples
  spiWrite(FIFO_CTRL_REG, 0x20 | 0x1f);

  delay(1000);
}

void loop() {
  spiRead(OUT_X_L_A);
  delay(20);
}

char spiRead(char address) {
  char dest;

  // Enable data transfer
  digitalWrite(slaveSelectPin, LOW);

  // Send read instruction to register and read it to dest
  SPI.transfer(READ | address);
  dest = SPI.transfer(0x00);

  // Disable data transfer
  digitalWrite(slaveSelectPin, HIGH);
  
  Serial.println(dest, HEX);
  return dest;
}

void spiWrite(char address, char data) {

  // Enable data transfer
  digitalWrite(slaveSelectPin, LOW);

  SPI.transfer(address);
  SPI.transfer(data);

  // Disable data transfer
  digitalWrite(slaveSelectPin, HIGH);
}

void accelerometerInit() {
  spiWrite(CTRL_REG0_XM, 0b00000000);
  spiWrite(CTRL_REG1_XM, 0b01100001);
  spiWrite(CTRL_REG2_XM, 0b00000000);
  spiWrite(CTRL_REG3_XM, 0b00001000);
}

