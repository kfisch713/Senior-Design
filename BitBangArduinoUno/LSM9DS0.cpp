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
LSM9DS0::LSM9DS0(int ss_ag, int ss_m, int mosi, int miso, int sck)
{
  SS_AG_PIN = ss_ag;
  SS_M_PIN = ss_m;
  MOSI_PIN = mosi;
  MISO_PIN = miso;
  SCK_PIN = sck;
  
  pinMode(SS_AG_PIN, OUTPUT);
  pinMode(SS_M_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SCK_PIN, OUTPUT);

  digitalWrite(SS_AG_PIN, HIGH);
  digitalWrite(SS_M_PIN, HIGH);
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
byte LSM9DS0::spi_read_byte_ag(byte address)
{
  byte receive = 0x00;
  byte mod_address = address | READ;
  
  digitalWrite(SS_AG_PIN, LOW);

  /* Write the address to the device. */
  spi_transfer(mod_address);

  /* Get the output from the addressed register. */
  receive = spi_transfer(0x00);
  
  digitalWrite(SS_AG_PIN, HIGH);
  return receive;
}

/**
 * Reads a magnetometer register and returns the value.
 */
byte LSM9DS0::spi_read_byte_m(byte address)
{
  byte receive = 0x00;
  byte mod_address = address | READ;
  
  digitalWrite(SS_M_PIN, LOW);

  /* Write the address to the device. */
  spi_transfer(mod_address);

  /* Get the output from the addressed register. */
  receive = spi_transfer(0x00);
  
  digitalWrite(SS_M_PIN, HIGH);
  return receive;
}

/**
 * Read a specified number of bytes from the device.
 */
void LSM9DS0::spi_read_bytes_ag(byte address, byte* data, byte count)
{
  /* Enabel data transfer to device. */
  digitalWrite(SS_AG_PIN, LOW);
  
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
  digitalWrite(SS_AG_PIN, HIGH);
}

/**
 * Read a specified number of bytes from the device.
 */
void LSM9DS0::spi_read_bytes_m(byte address, byte* data, byte count)
{
  /* Enabel data transfer to device. */
  digitalWrite(SS_M_PIN, LOW);
  
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
  digitalWrite(SS_M_PIN, HIGH);
}

/**
 * Writes a value to a specified register.
 */
void LSM9DS0::spi_write_ag(byte address, byte data)
{
  /* Enable data transfer to the device. */
  digitalWrite(SS_AG_PIN, LOW);

  /* Write the address to the device. */
  spi_transfer(address);

  /* Write data to the addressed register. */
  spi_transfer(data);

  /* End data transfer to the device. */
  digitalWrite(SS_AG_PIN, HIGH);
}

/**
 * Writes a value to a specified register.
 */
void LSM9DS0::spi_write_m(byte address, byte data)
{
  /* Enable data transfer to the device. */
  digitalWrite(SS_M_PIN, LOW);

  /* Write the address to the device. */
  spi_transfer(address);

  /* Write data to the addressed register. */
  spi_transfer(data);

  /* End data transfer to the device. */
  digitalWrite(SS_M_PIN, HIGH);
}

/**************************************************************************************/
/**********************  Accelerometer functions and whatnot. *************************/
/**************************************************************************************/

/**
 * Initialize data transfer registers for accelerometer output.
 */
void LSM9DS0::init_accelerometer()
{
  /* No decimation        (00)  */
  /* All axis activated   (111) */
  /* Default zeros        (000) */
  spi_write_ag(CTRL_REG5_XL, 0b00111000);

  /* Sample Rate   952hz  (110) */
  /* Scale         8g     (11)  */
  /* Bandwidth     408hz  (000) */
  spi_write_ag(CTRL_REG6_XL, 0b11011000);
  
  /* High res off         (0)   */
  /* High res BW off      (00)  */
  /* Default zeros        (00)  */
  /* Filter bypass        (0)   */
  /* Default zero         (0)   */
  /* Filter off           (0)   */
  spi_write_ag(CTRL_REG7_XL, 0b00000000);
}

/**
 * Read and print raw accelerometer data.
 */
void LSM9DS0::print_raw_accelerometer()
{
  uint8_t temp[6];

  /* Read data from all accelerometer output registers. */
  spi_read_bytes_ag(OUT_X_L_XL, temp, 6);

  /* Store it into various variables. */
  uint16_t ax = (temp[1] << 8) | temp[0];
  uint16_t ay = (temp[3] << 8) | temp[2];
  uint16_t az = (temp[5] << 8) | temp[4];

  /* Print that shit. */
  Serial.print("aX: ");
  Serial.print(ax);
  Serial.print(" aY: ");
  Serial.print(ay);
  Serial.print(" aZ: ");
  Serial.println(az);
}

/**
 * Read and print calculated accelerometer data.
 */
void LSM9DS0::print_calculated_accelerometer()
{
  uint8_t temp[6];
  float a_res = 8.0 / 32768.0;
  
  /* Read data from all accelerometer output registers. */
  spi_read_bytes_ag(OUT_X_L_XL, temp, 6);

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
/************************  Gyroscope functions and whatnot. ***************************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for gyroscope output.
 */
void LSM9DS0::init_gyroscope()
{
  /* Sample Rate   952hz  (110) */
  /* Scale         245dps (00)  */
  /* Default zero         (0)   */
  /* Bandwidth     33hz   (00)  */
  spi_write_ag(CTRL_REG1_G, 0b11000000);
  
  /* Default zeros       (0000) */
  /* Interrupt select    (00)   */
  /* Output select       (00)   */
  spi_write_ag(CTRL_REG2_G, 0b00000000);
  
  /* Low power off       (0)    */
  /* High pass off       (0)    */
  /* Default zeros       (00)   */
  /* High pass off       (000)  */
  spi_write_ag(CTRL_REG3_G, 0b00000000);
  
  /* Default zeros       (00)   */
  /* Enable all axis     (111)  */
  /* Default zero        (0)    */
  /* Interrupts not used (00)   */
  spi_write_ag(CTRL_REG4, 0b00111000);
  
  /* Default zeros       (00)   */
  /* Positive signs      (000)  */
  /* Orient              (000)  */
  spi_write_ag(ORIENT_CFG_G, 0b00000000);
}

/*
 * Read and print raw gyroscope data.
 */
void LSM9DS0::print_raw_gyroscope()
{
  uint8_t temp[6];
  
  /* Read data from all accelerometer output registers. */
  spi_read_bytes_ag(OUT_X_L_G, temp, 6);

  /* Store it into various variables. */
  int gx = (temp[1] << 8) | temp[0];
  int gy = (temp[3] << 8) | temp[2];
  int gz = (temp[5] << 8) | temp[4];

  /* Print that shit. */
  Serial.print("gX: ");
  Serial.print(gx);
  Serial.print(" gY: ");
  Serial.print(gx);
  Serial.print(" gZ: ");
  Serial.println(gx);
}

/*
 * Read and print calculated gyroscope data.
 */
void LSM9DS0::print_calculated_gyroscope()
{
  uint8_t temp[6];
  double gres = 0.00875;
  
  /* Read data from all accelerometer output registers. */
  spi_read_bytes_ag(OUT_X_L_G, temp, 6);

  /* Store it into various variables. */
  int gx = (temp[1] << 8) | temp[0];
  int gy = (temp[3] << 8) | temp[2];
  int gz = (temp[5] << 8) | temp[4];

  double gx_calc = (double)gx * gres;
  double gy_calc = (double)gy * gres;
  double gz_calc = (double)gz * gres;

  /* Print that shit. */
  Serial.print("gX: ");
  Serial.print(gx_calc);
  Serial.print(" gY: ");
  Serial.print(gy_calc);
  Serial.print(" gZ: ");
  Serial.println(gz_calc);
}

/**************************************************************************************/
/***********************  Magnetometer functions and whatnot. *************************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for magnetometer output.
 */
void LSM9DS0::init_magnetometer()
{
  /* Temp comp disable  (0) */
  /* XY high pref      (11) */
  /* Output data 80hz (111) */
  /* Disable fast odr   (0) */
  /* Self test off      (0) */
  Serial.println(spi_read_byte_m(CTRL_REG1_M), BIN);
  spi_write_m(CTRL_REG1_M, 0b01111100);
  Serial.println(spi_read_byte_m(CTRL_REG1_M), BIN);

  /* Default zero       (0) */
  /* Scale 4gauss      (00) */
  /* Default zero       (0) */
  /* Keep memory        (0) */
  /* Reset default      (0) */
  /* Default zeros     (00) */
  spi_write_m(CTRL_REG2_M, 0b00000000);

  /* Disable I2C        (0) */
  /* Default zero       (0) */
  /* Low power mode off (0) */
  /* Default zeros     (00) */
  /* SPI read/write     (1) */
  /* Continuous conv   (00) */
  spi_write_m(CTRL_REG3_M, 0b00000000);

  /* Default zeros   (0000) */
  /* Z High preform    (11) */
  /* Little endian      (0) */
  /* Default zero       (0) */
  spi_write_m(CTRL_REG4_M, 0b00001100);

  /* Default zero       (0) */
  /* Update continuous  (0) */
  /* Default zeros (000000) */
  spi_write_m(CTRL_REG5_M, 0b00000000);
}

/*
 * Read and print raw magnetometer data.
 */
void LSM9DS0::print_raw_magnetometer()
{
  uint8_t temp[6];
  
  /* Read data from all accelerometer output registers. */
  spi_read_bytes_m(OUT_X_L_M, temp, 6);

  /* Store it into various variables. */
  int mx = (temp[1] << 8) | temp[0];
  int my = (temp[3] << 8) | temp[2];
  int mz = (temp[5] << 8) | temp[4];

  /* Print that shit. */
  Serial.print("mX: ");
  Serial.print(mx);
  Serial.print(" mY: ");
  Serial.print(my);
  Serial.print(" mZ: ");
  Serial.println(mz);
}

void LSM9DS0::print_calculated_magnetometer()
{
  uint8_t temp[6];
  double mres = 0.00014;
  
  /* Read data from all accelerometer output registers. */
  spi_read_bytes_ag(OUT_X_L_M, temp, 6);

  /* Store it into various variables. */
  int mx = (temp[1] << 8) | temp[0];
  int my = (temp[3] << 8) | temp[2];
  int mz = (temp[5] << 8) | temp[4];

  double mx_calc = (double)mx * mres;
  double my_calc = (double)my * mres;
  double mz_calc = (double)mz * mres;

  /* Print that shit. */
  Serial.print("mX: ");
  Serial.print(mx_calc);
  Serial.print(" mY: ");
  Serial.print(my_calc);
  Serial.print(" mZ: ");
  Serial.println(mz_calc);
}


