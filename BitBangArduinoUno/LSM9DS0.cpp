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
  
  /* Send data to the device and receive data from the device. */
  for(int i = 0; i < 8; i++) {
    digitalWrite(SCK_PIN, LOW);
    bitWrite(receive, 7-i, digitalRead(MISO_PIN));
    digitalWrite(MOSI_PIN, bitRead(data, 7-i));
    digitalWrite(SCK_PIN, HIGH);
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

  /* Write the address to the device. */
  spi_transfer(mod_address);

  /* Get the output from the addressed register. */
  receive = spi_transfer(0x00);
  
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
    spi_transfer(0x80 | address);
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

  /* Write the address to the device. */
  spi_transfer(address);

  /* Write data to the addressed register. */
  spi_transfer(data);

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
  spi_read_bytes(OUT_X_L_XL, temp, 6);

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
  spi_read_bytes(OUT_X_L_XL, temp, 6);

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
  /* No decimation        (00)  */
  /* All axis activated   (111) */
  /* Default zeros        (000) */
  spi_write(CTRL_REG5_XL, 0b00111000);

  /* Sample Rate   952hz  (110) */
  /* Scale         2g     (00)  */
  /* Bandwidth     408hz  (000) */
  Serial.println(spi_read_byte(CTRL_REG6_XL), BIN);
  spi_write(CTRL_REG6_XL, 0b11000000);
  Serial.println(spi_read_byte(CTRL_REG6_XL), BIN);
  
  /* High res off         (0)   */
  /* High res BW off      (00)  */
  /* Default zeros        (00)  */
  /* Filter bypass        (0)   */
  /* Default zero         (0)   */
  /* Filter off           (0)   */
  spi_write(CTRL_REG7_XL, 0b00000000);
}

