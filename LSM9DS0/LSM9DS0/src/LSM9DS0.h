/*
 * LSM9DS0.h
 *
 * Created: 11/19/2016 4:25:16 PM
 *  Author: Michael
 */ 


#ifndef LSM9DS0_H_
#define LSM9DS0_H_

#define SLAVE_SELECT_PIN  PIN_LP_GPIO_12

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

/*
 * Custom SPI read function.  Reads a single register (8 bits) from the 
 * LSM9DS0 sensor.
 */
uint8_t hw_spi_read(struct spi_module* master_instance, struct spi_slave_inst* slave_instance, uint8_t reg_to_read);

/*
 * Configures the accelerometer control registers for the LSM9DS0.
 */
void configure_xm_ctrl_regs(void);

/*
 * Configure UART console.
 */
void configure_console(struct uart_module* console_instance);

/*
 * Configure the SPI hardware for the LSM9DS0
 */
void configure_spi(struct spi_module* master_instance, struct spi_slave_inst* slave_instance);

#endif /* LSM9DS0_H_ */