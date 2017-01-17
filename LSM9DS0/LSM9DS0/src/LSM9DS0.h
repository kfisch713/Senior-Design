/*
 * LSM9DS0.h
 *
 * Created: 11/19/2016 3:26:03 PM
 * Author: Michael Linthicum
 */ 


#ifndef LSM9DS0_H_
#define LSM9DS0_H_

#define READ            0x80
#define MULTIPLE_READ   0xC0
#define WHO_AM_I_XM     0x0f
#define OUT_X_L_A       0x28
#define OUT_X_H_A       0x29
#define OUT_Y_L_A       0x2A
#define OUT_Y_H_A       0x2B
#define OUT_Z_L_A       0x2C
#define OUT_Z_H_A       0x2D
#define FIFO_CTRL_REG   0x2E
#define CTRL_REG0_XM    0x1f
#define CTRL_REG1_XM    0x20
#define CTRL_REG2_XM    0x21
#define CTRL_REG3_XM    0x22
#define FIFO_SRC_REG    0x2F

#define SSC_PIN  PIN_LP_GPIO_12 /* EXT1 15 CSXM  */
#define SCK_PIN  PIN_LP_GPIO_10 /* EXT1 18 SCL   */
#define MISO_PIN PIN_LP_GPIO_13 /* EXT1 17 SDOXM */
#define MOSI_PIN PIN_LP_GPIO_11 /* EXT1 16 SDA   */

/* All possible output data rates of the accelerometer. */
typedef enum {
	A_POWER_DOWN, /* Power down mode (0000) */
	A_ODR_3125,   /* 3.125 Hz        (0001) */
	A_ODR_625,    /* 6.25 Hz         (0010) */
	A_ODR_125,    /* 12.5 Hz         (0011) */
	A_ODR_25,     /* 25 Hz           (0100) */
	A_ODR_50,     /* 50 Hz           (0101) */
	A_ODR_100,    /* 100 Hz          (0110) */
	A_ODR_200,    /* 200 Hz          (0111) */
	A_ODR_400,    /* 400 Hz          (1000) */
	A_ODR_800,    /* 800 Hz          (1001) */
	A_ODR_1600    /* 1600 Hz         (1010) */
} a_odr;

/* All possible scale values for the accelerometer. */
typedef enum {
	A_SCALE_2G,  /* 2G  (000) */
	A_SCALE_4G,  /* 4G  (001) */
	A_SCALE_6G,  /* 6G  (010) */
	A_SCALE_8G,  /* 8G  (011) */
	A_SCALE_16G, /* 16G (100) */
} a_scale;

/*
 * Transfers a byte to the device and reads and returns the byte transfered
 * out of the device.  The simplest SPI operation.
 */
uint8_t spi_transfer(uint8_t data);

/*
 *  Writes the value specified in uint8_t data to the register specified by
 *  uint8_t address.
 */
void my_spi_write(uint8_t address, uint8_t data);

/*
 *  Read a byte through SPI
 */
uint8_t spi_read_byte(uint8_t address);

/* 
 * Read a certain number of bytes from a register.
 */
void spi_read_bytes(uint8_t address, uint8_t* data, uint8_t count);

/*
 * Reads accelerometer values and prints them raw.
 */
void print_raw_accelerometer(void);

/*
 * Reads and calculates accelerometer values and prints them.
 */
void print_calculated_accelerometer(a_odr rate, a_scale scale);

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

/* 
 * Set the output data rate for the accelerometer.
 */
void init_accelerometer_odr(a_odr rate);

/* 
 * Set the scale for the accelerometer.
 */
void init_accelerometer_scale(a_scale scale);

/*
 * Initialize data transfer registers for accelerometer output.
 */
void init_accelerometer(void);

/*
 *  Configure UART console.
 */
void configure_console(struct uart_module* console_instance);

/*
 *  Configure GPIO pins for SPI
 */
void configure_gpio(void);


#endif /* LSM9DS0_H_ */