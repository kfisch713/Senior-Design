/*
 * LSM9DS1.h
 *
 * Created: 3/21/2017 2:36:06 PM
 *  Author: Michael
 */ 


#ifndef LSM9DS1_H_
#define LSM9DS1_H_

#define READ            0x80
#define MULTIPLE_READ   0xC0
#define CTRL_REG1_G		0x10
#define CTRL_REG2_G		0x11
#define CTRL_REG3_G		0x12
#define ORIENT_CFG_G	0x13
#define CTRL_REG4		0x1E
#define CTRL_REG5_XL	0x1F
#define CTRL_REG6_XL	0x20
#define CTRL_REG7_XL	0x21
#define OUT_X_L_XL		0x28
#define OUT_X_L_G		0x18
#define WHO_AM_I_XM     0x0f

#define AG_SSC_PIN  PIN_LP_GPIO_12 /* EXT1 15 CSXM  */
#define AG_SCK_PIN  PIN_LP_GPIO_10 /* EXT1 18 SCL   */
#define AG_MISO_PIN PIN_LP_GPIO_13 /* EXT1 17 SDOXM */
#define AG_MOSI_PIN PIN_LP_GPIO_11 /* EXT1 16 SDA   */

#define M_SSC_PIN	PIN_LP_GPIO_16 /* EXT3 15 CSG  */
#define M_SCK_PIN	PIN_LP_GPIO_10 /* EXT1 18 SCL  */
#define M_MISO_PIN	PIN_LP_GPIO_18 /* EXT3 17 SDOG */
#define M_MOSI_PIN	PIN_LP_GPIO_11 /* EXT1 16 SDA  */

/*
 * Transfers a byte to the device and reads and returns the byte transfered
 * out of the device.  The simplest SPI operation.
 */
uint8_t ag_transfer(uint8_t data);

/*
 * Transfers a byte to the device and reads and returns the byte transfered
 * out of the device.  The simplest SPI operation.
 */
uint8_t m_transfer(uint8_t data);

/*
 *  Writes the value specified in uint8_t data to the accelerometer/gyroscope 
 *  register specified by uint8_t address.
 */
void ag_write(uint8_t address, uint8_t data);

/*
 *  Writes the value specified in uint8_t data to the magnetometer register 
 *  specified by uint8_t address.
 */
void m_write(uint8_t address, uint8_t data);

/*
 *  Read a byte through SPI from the accelerometer/gyroscope registers.
 */
uint8_t ag_read_byte(uint8_t address);

/*
 *  Read a byte through SPI from the magnetometer registers.
 */
uint8_t m_read_byte(uint8_t address);

/*
 * Read a specified number of bytes from the accelerometer/gyroscope
 * registers.
 */
void ag_read_bytes(uint8_t address, uint8_t* data, uint8_t count);

/*
 * Read a specified number of bytes from the magnetometer
 * registers.
 */
void m_read_bytes(uint8_t address, uint8_t* data, uint8_t count);

/*
 *  Writes a high value to the bit corresponding with uint8_t bit.
 *  The bits are ordered from right to left, zero indexed.
 */
uint8_t bit_write_high(uint8_t data, uint8_t bit);

/*
 *  Reads the bit value corresponding with uint8_t bit.
 *  This bits are ordered from right to left, zero indexed.
 */
uint8_t bit_read(uint8_t data, uint8_t bit);

/**************************************************************************************/
/***********  Accelerometer register initiation and other such nonsense. **************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for accelerometer output.
 */
void init_accelerometer(void);

/*
 * Read and print raw accelerometer data.
 */
void get_raw_accelerometer(uint16_t *output);

/**************************************************************************************/
/************  Gyroscope register initiation and other such nonsense. *****************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for gyroscope output.
 */
void init_gyroscope(void);

/*
 * Read and print raw gyroscope data.
 */
void get_raw_gyroscope(uint16_t* output);

/*
 *  Configure UART console.
 */
void configure_console(struct uart_module* console_instance);

/*
 *  Configure GPIO pins for SPI
 */
void configure_gpio(void);


#endif /* LSM9DS1_H_ */