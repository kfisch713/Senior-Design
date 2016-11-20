/**
 *  @file    LSM9DS0.cpp
 *  @author  Michael Linthicum
 *  @date    19 November 2016
 *  @brief   Library for interfacing with LSM9DS0 over bit banged SPI.
 */

#include "LSM9DS0.h"

/**
 * Initialize an LSM9DS0 object and set the slave select pin high.
 */
LSM9DS0::LSM9DS0(int ss, int mosi, int miso, int sck)
{
  SS_PIN = ss;
  MOSI_PIN = mosi;
  MISO_PIN = miso;
  SCK_PIN = sck;
  
  pinMode(SS_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SCK_PIN, OUTPUT);

  digitalWrite(SS_PIN, HIGH);
}

/**
 * Simple SPI transfer that transfers data to the device and reads
 * data from the device.
 */
byte LSM9DS0::spi_transfer(byte data)
{
  byte receive = 0x00;
  digitalWrite(SCK_PIN, HIGH);

  /* Send data to the device and receive data from the device. */
  for(int i = 0; i < 8; i++) {
    digitalWrite(SCK_PIN, LOW);
    digitalWrite(MOSI_PIN, bitRead(data, 7-i));
    digitalWrite(SCK_PIN, HIGH);
    bitWrite(receive, 7-i, digitalRead(MISO_PIN));
  }
  
  return receive;
}

/**
 * Reads a register and returns the value.
 */
byte LSM9DS0::spi_read_byte(byte address)
{
  byte receive = 0x00;
  byte mod_address = address | READ;
  
  digitalWrite(SS_PIN, LOW);
  digitalWrite(SCK_PIN, HIGH);

  /* Write the address to the device. */
  for(int i = 0; i < 8; i++){
    spi_transfer(mod_address);
  }

  /* Get the output from the addressed register. */
  for(int i = 0; i < 8; i++){
    receive = spi_transfer(0x00);
  }
  
  digitalWrite(SS_PIN, HIGH);
  return receive;
}

/**
 * Read a specified number of bytes from the device.
 */
void LSM9DS0::spi_read_bytes(byte address, byte* data, byte count)
{
  /* Enabel data transfer to device. */
  digitalWrite(SS_PIN, LOW);
  
  if(count > 1){
    /* If count is greater than one modify the address to reflect that. */
    spi_transfer(0xC0 | address);
  } else {
    /* Modify the address to reflect a single byte read. */
    spi_transfer(0x80 | address);
  }

  /* Read count number of bytes from the address. */
  for(int i = 0; i < count; i++) {
    data[i] = spi_transfer(0x00);
  }

  /* End data transfer to the device. */
  digitalWrite(SS_PIN, HIGH);
}

/**
 * Writes a value to a specified register.
 */
void LSM9DS0::spi_write(byte address, byte data)
{
  /* Enable data transfer to the device. */
  digitalWrite(SS_PIN, LOW);
  digitalWrite(SCK_PIN, HIGH);

  /* Write the address to the device. */
  for(int i = 0; i < 8; i++){
    spi_transfer(address);
  }

  /* Write data to the addressed register. */
  for(int i = 0; i < 8; i++){
    spi_transfer(data);
  }

  /* End data transfer to the device. */
  digitalWrite(SS_PIN, HIGH);
}

/**
 * Read and print raw accelerometer data.
 */
void LSM9DS0::print_raw_accelerometer()
{
  uint8_t temp[6];

  /* Read data from all accelerometer output registers. */
  spi_read_bytes(OUT_X_L_A, temp, 6);

  /* Store it into various variables. */
  uint16_t ax = (temp[1] << 8) | temp[0];
  uint16_t ay = (temp[3] << 8) | temp[2];
  uint16_t az = (temp[5] << 8) | temp[4];

  /* Print that shit. */
  Serial.print("X: ");
  Serial.print(ax);
  Serial.print(" Y: ");
  Serial.print(ay);
  Serial.print(" Z: ");
  Serial.println(az);
}

/**
 * Read and print raw accelerometer data.
 */
void LSM9DS0::print_calculated_accelerometer()
{
  uint8_t temp[6];
  float a_res = accelerometer_scale == A_SCALE_16G ? 16.0 / 32768.0 : (((float) accelerometer_scale + 1.0) * 2.0) / 32768.0;
  
  /* Read data from all accelerometer output registers. */
  spi_read_bytes(OUT_X_L_A, temp, 6);

  /* Store it into various variables. */
  int ax = (temp[1] << 8) | temp[0];
  int ay = (temp[3] << 8) | temp[2];
  int az = (temp[5] << 8) | temp[4];

  float ax_calc = a_res * (float)ax;
  float ay_calc = a_res * (float)ay;
  float az_calc = a_res * (float)az;

  /* Print that shit. */
  Serial.print("X: ");
  Serial.print(ax_calc, 2);
  Serial.print(" Y: ");
  Serial.print(ay_calc, 2);
  Serial.print(" Z: ");
  Serial.println(az_calc, 2);
}

/**************************************************************************************/
/******************  Register initiation and other such nonsense. *********************/
/**************************************************************************************/

/**
 * Initialize data transfer registers for accelerometer output.
 */
void LSM9DS0::init_accelerometer()
{
  /* CTRL_REG0_XM (0x1F) (Default value: 0x00)
   *  Bits (7-0): BOOT FIFO_EN WTM_EN 0 0 HP_CLICK HPIS1 HPIS2
   *  BOOT - Reboot memory content (0: normal, 1: reboot)
   *  FIFO_EN - Fifo enable (0: disable, 1: enable)
   *  WTM_EN - FIFO watermark enable (0: disable, 1: enable)
   *  HP_CLICK - HPF enabled for click (0: filter bypassed, 1: enabled)
   *  HPIS1 - HPF enabled for interrupt generator 1 (0: bypassed, 1: enabled)
   *  HPIS2 - HPF enabled for interrupt generator 2 (0: bypassed, 1 enabled)
   */
  spi_write(CTRL_REG0_XM, 0x00);

  /* CTRL_REG1_XM (0x20) (Default value: 0x07)
   *  Bits (7-0): AODR3 AODR2 AODR1 AODR0 BDU AZEN AYEN AXEN
   *  AODR[3:0] - select the acceleration data rate:
   *    0000=power down, 0001=3.125Hz, 0010=6.25Hz, 0011=12.5Hz, 
   *    0100=25Hz, 0101=50Hz, 0110=100Hz, 0111=200Hz, 1000=400Hz,
   *    1001=800Hz, 1010=1600Hz, (remaining combinations undefined).
   *  BDU - block data update for accel AND mag
   *    0: Continuous update
   *    1: Output registers aren't updated until MSB and LSB have been read.
   *  AZEN, AYEN, and AXEN - Acceleration x/y/z-axis enabled.
   *    0: Axis disabled, 1: Axis enabled 
   */
  spi_write(CTRL_REG1_XM, 0x57);

  /* CTRL_REG2_XM (0x21) (Default value: 0x00)
   *  Bits (7-0): ABW1 ABW0 AFS2 AFS1 AFS0 AST1 AST0 SIM
   *  ABW[1:0] - Accelerometer anti-alias filter bandwidth
   *    00=773Hz, 01=194Hz, 10=362Hz, 11=50Hz
   *  AFS[2:0] - Accel full-scale selection
   *    000=+/-2g, 001=+/-4g, 010=+/-6g, 011=+/-8g, 100=+/-16g
   *  AST[1:0] - Accel self-test enable
   *    00=normal (no self-test), 01=positive st, 10=negative st, 11=not allowed
   *  SIM - SPI mode selection
   *    0=4-wire, 1=3-wire
   */
  spi_write(CTRL_REG2_XM, 0x00);

  /* CTRL_REG3_XM is used to set interrupt generators on INT1_XM
   * Bits (7-0): P1_BOOT P1_TAP P1_INT1 P1_INT2 P1_INTM P1_DRDYA P1_DRDYM P1_EMPTY
   */
  spi_write(CTRL_REG3_XM, 0x04);
}

/**
 * Set the output data rate for the accelerometer.
 */
void LSM9DS0::init_accelerometer_odr(a_odr rate)
{
  accelerometer_odr = rate;
  
  /* Read the current CTRL_REG1_XM value. */
  byte temp = spi_read_byte(CTRL_REG1_XM);

  /* Mask the current odr bits. */
  temp &= 0xFF ^ (0xF << 4);

  /* Shift in wanted odr bits. */
  temp |= (rate << 4);

  /* Write back to CTRL_REG1_XM register. */
  spi_write(CTRL_REG1_XM, temp);
}

/**
 * Set the scale for the data from the acceleromter.
 */
void LSM9DS0::init_accelerometer_scale(a_scale scale)
{
  accelerometer_scale = scale;
  
  /* Read the current CTRL_REG2_XM value. */
  byte temp = spi_read_byte(CTRL_REG2_XM);

  /* Mask the current scale bits. */
  temp &= 0xFF ^ (0x3 << 3);

  /* Shift in wanted scale bits. */
  temp |= (scale << 3);

  /* Write back to the CTRL_REG2_XM register. */
  spi_write(CTRL_REG2_XM, temp);
}

