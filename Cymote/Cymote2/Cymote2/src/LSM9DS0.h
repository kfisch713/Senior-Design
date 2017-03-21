/*
 * BitBang.h
 *
 * Created: 1/23/2017 5:51:19 PM
 *  Author: Michael
 */ 


#ifndef LSM9DS0_H_
#define LSM9DS0_H_

#define READ            0x80
#define MULTIPLE_READ   0xC0
#define WHO_AM_I_XM     0x0f
#define OUT_X_L_A       0x28
#define OUT_X_L_M		0x08
#define OUT_X_L_G		0x28
#define FIFO_CTRL_REG   0x2E
#define CTRL_REG0_XM    0x1f
#define CTRL_REG1_XM    0x20
#define CTRL_REG2_XM    0x21
#define CTRL_REG3_XM    0x22
#define CTRL_REG4_XM	0x23
#define CTRL_REG5_XM	0x24
#define CTRL_REG6_XM	0x25
#define CTRL_REG7_XM	0x26
#define INT_CTRL_REG_M	0x12
#define CTRL_REG1_G		0x20
#define CTRL_REG2_G		0x21
#define CTRL_REG3_G		0x22
#define CTRL_REG4_G		0x23
#define CTRL_REG5_G		0x24
#define FIFO_SRC_REG    0x2F

#define AM_SSC_PIN  PIN_LP_GPIO_12 /* EXT1 15 CSXM  */
#define AM_SCK_PIN  PIN_LP_GPIO_10 /* EXT1 18 SCL   */
#define AM_MISO_PIN PIN_LP_GPIO_13 /* EXT1 17 SDOXM */
#define AM_MOSI_PIN PIN_LP_GPIO_11 /* EXT1 16 SDA   */

#define G_SSC_PIN	PIN_LP_GPIO_16 /* EXT3 15 CSG  */
#define G_SCK_PIN	PIN_LP_GPIO_10 /* EXT1 18 SCL  */
#define G_MISO_PIN	PIN_LP_GPIO_18 /* EXT3 17 SDOG */
#define G_MOSI_PIN	PIN_LP_GPIO_11 /* EXT1 16 SDA  */

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

/* All possible output data rates of the magnetometer. */
typedef enum {
	M_ODR_3125, /* 3.125 Hz (000) */
	M_ODR_625,	/* 6.25 Hz	(001) */
	M_ODR_125,	/* 1.25 Hz	(010) */
	M_ODR_25,	/* 25 Hz	(011) */
	M_ODR_50,	/* 50 Hz	(100) */
	M_ODR_100,	/* 100 Hz	(101) */
} m_odr;

/* All possible scale values for the magnetometer. */
typedef enum {
	M_SCALE_2GS,  /* 2GS  (00) */
	M_SCALE_4GS,  /* 4GS  (01) */
	M_SCALE_8GS,  /* 8GS  (10) */
	M_SCALE_12Gs, /* 12Gs (11) */
} m_scale;

/* All possible output data rates of the gyroscope. */
typedef enum {
	G_ODR_95_BW_125  = 0x0, /* 95 Hz  12.5 */
	G_ODR_95_BW_25   = 0x1, /* 95 Hz  25   */
	G_ODR_190_BW_125 = 0x4, /* 190 Hz 12.5 */
	G_ODR_190_BW_25  = 0x5, /* 190 Hz 25   */
	G_ODR_190_BW_50  = 0x6, /* 190 Hz 50   */
	G_ODR_190_BW_70  = 0x7, /* 190 Hz 70   */
	G_ODR_380_BW_20  = 0x8, /* 380 Hz 20   */
	G_ODR_380_BW_25  = 0x9, /* 380 Hz 25   */
	G_ODR_380_BW_50  = 0xA, /* 380 Hz 50   */
	G_ODR_380_BW_100 = 0xB, /* 380 Hz 100  */
	G_ODR_760_BW_30  = 0xC, /* 760 Hz 30   */
	G_ODR_760_BW_35  = 0xD, /* 760 Hz 35   */
	G_ODR_760_BW_50  = 0xE, /* 760 Hz 50   */
	G_ODR_760_BW_100 = 0xF, /* 760 Hz 100  */
} g_odr;

/* All possible scale values for the gyroscope. */
typedef enum {
	G_SCALE_245DPS,  /* 245 dps  (00) */
	G_SCALE_500DPS,	 /* 500 dps  (01) */
	G_SCALE_2000DPS, /* 2000 dps (10) */
} g_scale;

/*
 * Transfers a byte to the device and reads and returns the byte transfered
 * out of the device.  The simplest SPI operation.
 */
uint8_t am_transfer(uint8_t data);

/*
 * Transfers a byte to the device and reads and returns the byte transfered
 * out of the device.  The simplest SPI operation.
 */
uint8_t g_transfer(uint8_t data);

/*
 *  Writes the value specified in uint8_t data to the accelerometer/magnetometer 
 *  register specified by uint8_t address.
 */
void am_write(uint8_t address, uint8_t data);

/*
 *  Writes the value specified in uint8_t data to the gyroscope register 
 *  specified by uint8_t address.
 */
void g_write(uint8_t address, uint8_t data);

/*
 *  Read a byte through SPI from the accelerometer/magnetometer registers.
 */
uint8_t am_read_byte(uint8_t address);

/*
 *  Read a byte through SPI from the gyroscope registers.
 */
uint8_t g_read_byte(uint8_t address);

/*
 * Read a specified number of bytes from the accelerometer/magnetometer
 * registers.
 */
void am_read_bytes(uint8_t address, uint8_t* data, uint8_t count);

/*
 * Read a specified number of bytes from the gyroscope
 * registers.
 */
void g_read_bytes(uint8_t address, uint8_t* data, uint8_t count);

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
 * Set the output data rate for the accelerometer.
 */
void init_accelerometer_odr(a_odr rate);

/* 
 * Set the scale for the accelerometer.
 */
void init_accelerometer_scale(a_scale scale);

/*
 * Reads accelerometer values and prints them raw.
 */
void get_raw_accelerometer(uint16_t *output);

/*
 * Reads and calculates accelerometer values and prints them.
 */
void print_calculated_accelerometer(a_odr rate, a_scale scale);

/**************************************************************************************/
/***********  Magnetometer register initiation and other such nonsense. ***************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for magnetometer output.
 */
void init_magnetometer(void);

/*
 * Set the output data rate for the magnetometer.
 */
void init_magnetometer_odr(m_odr rate);

/*
 * Set the scale for the data from the magnetometer.
 */
void init_magnetometer_scale(m_scale scale);

/*
 * Read raw magnetometer data.
 */
void get_raw_magnetometer(uint16_t *output);

/*
 * Read and print raw magnetometer data.
 */
void print_raw_magnetometer(void);

/**************************************************************************************/
/***********  Magnetometer register initiation and other such nonsense. ***************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for gyroscope output.
 */
void init_gyroscope(void);

/*
 * Set the output data rate for the gyroscope.
 */
void init_gyroscope_odr(g_odr rate);

/*
 * Set the scale for the data from the gyroscope.
 */
void init_gyroscope_scale(g_scale scale);

/*
 * Read raw gyroscope data.
 */
void get_raw_gyroscope(uint16_t *output);

/*
 * Read and print raw gyroscope data.
 */
void print_raw_gyroscope(void);

/***********************************************************************/
/*************** Board and Console set up nonsense *********************/
/***********************************************************************/

/*
 * Configures the accelerometer control registers for the LSM9DS0.
 */
void configure_xm_ctrl_regs(void);

/*
 * Configure UART console.
 */
void configure_console(struct uart_module* console_instance);

/*
 *  Configure GPIO pins for SPI
 */
void configure_gpio(void);

#endif /* LSM9DS0_H_ */