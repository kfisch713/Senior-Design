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
 * 20 September 2016
 * Michael Linthicum
 */

#include<SPI.h>

#define READ            0x80
#define WHO_AM_I_XM     0x0f
#define OUT_X_L_A       0x28
#define FIFO_CTRL_REG   0x2E
#define CTRL_REG0_XM    0x1f
#define CTRL_REG1_XM    0x20
#define CTRL_REG2_XM    0x21
#define CTRL_REG3_XM    0x22

const SPISettings SPISetting = SPISettings(1000000, MSBFIRST, SPI_MODE1);

// Set pin 10 as the slave select (SS)
const int scPin = 10;

void setup() {
  Serial.begin(9600);
  pinMode(scPin, OUTPUT);
  
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
  delay(100);
}

/**
 * Method to read a register via SPI
 */
char spiRead(char address) {
  char dest;

  // Enable data transfer
  digitalWrite(scPin, LOW);

  // Send read instruction to register and read it to dest
  SPI.transfer(READ | address);
  dest = SPI.transfer(0x00);

  // Disable data transfer
  digitalWrite(scPin, HIGH);
  
  Serial.println(dest, HEX);
  return dest;
}

/**
 * Method to write a register via SPI
 */
void spiWrite(char address, char data) {

  // Enable data transfer
  digitalWrite(scPin, LOW);

  SPI.transfer(address);
  SPI.transfer(data);

  // Disable data transfer
  digitalWrite(scPin, HIGH);
}

/**
 * Initialize the accelerometer registers.
 */
void accelerometerInit() {
  
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
  spiWrite(CTRL_REG1_XM, 0b01100001);

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

