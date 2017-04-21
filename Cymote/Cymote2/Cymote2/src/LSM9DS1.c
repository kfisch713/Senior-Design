/*
 * LSM9DS1.c
 *
 * Created: 3/21/2017 2:35:54 PM
 *  Author: Michael
 */ 

#include <asf.h>
#include <LSM9DS1.h>

/*
 * Transfers a byte to the device and reads and returns the byte transfered
 * out of the device.  The simplest SPI operation.  SPI mode 0.
 */
uint8_t ag_transfer(uint8_t data)
{
	uint8_t receive = 0x00;
	
	/* Send data to the device and receive data. */
	for(int i = 0; i < 8; i++) {
		
		/* Clock low. */
		gpio_pin_set_output_level(AG_SCK_PIN, LOW);
		
		/* Read bit from MISO. */
		if(gpio_pin_get_input_level(AG_MISO_PIN) == HIGH) {
			receive = bit_write_high(receive, 7-i);
		}
		
		/* Write bit to MOSI. */
		if(bit_read(data, 7-i) > 0x00) {
			gpio_pin_set_output_level(AG_MOSI_PIN, HIGH);
		} else {
			gpio_pin_set_output_level(AG_MOSI_PIN, LOW);
		}
		
		/* Clock high. */
		gpio_pin_set_output_level(AG_SCK_PIN, HIGH);
		
	}
	return receive;
}

uint8_t m_transfer(uint8_t data)
{
	uint8_t receive = 0x00;
	
	/* Send data to the device and receive data. */
	for(int i = 0; i < 8; i++) {
		
		/* Clock low. */
		gpio_pin_set_output_level(M_SCK_PIN, LOW);
		
		/* Read bit from MISO. */
		if(gpio_pin_get_input_level(M_MISO_PIN) == HIGH) {
			receive = bit_write_high(receive, 7-i);
		}
		
		/* Write bit to MOSI. */
		if(bit_read(data, 7-i) > 0x00) {
			gpio_pin_set_output_level(M_MOSI_PIN, HIGH);
			} else {
			gpio_pin_set_output_level(M_MOSI_PIN, LOW);
		}
		
		/* Clock high. */
		gpio_pin_set_output_level(M_SCK_PIN, HIGH);
		
	}
	return receive;
}

/*
 *  Writes the value specified in uint8_t data to the accelerometer/gyroscope 
 *  register specified by uint8_t address.
 */
void ag_write(uint8_t address, uint8_t data)
{
	gpio_pin_set_output_level(AG_SSC_PIN, LOW);
	
	/* Write address */
	ag_transfer(address);
	
	/* Get response */
	ag_transfer(data);
	
	gpio_pin_set_output_level(AG_SSC_PIN, HIGH);
	return;
}

/*
 *  Writes the value specified in uint8_t data to the magnetometer register 
 *  specified by uint8_t address.
 */
void m_write(uint8_t address, uint8_t data)
{
	gpio_pin_set_output_level(M_SSC_PIN, LOW);
	
	/* Write address. */
	m_transfer(address);
	
	/* Get response. */
	m_transfer(data);
	
	gpio_pin_set_output_level(M_SSC_PIN, HIGH);
	return;
}

/*
 *  Read a byte through SPI from the accelerometer/gyroscope registers.
 */
uint8_t ag_read_byte(uint8_t address)
{
	uint8_t receive = 0x00;
	uint8_t mod_address = address | READ;
	
	gpio_pin_set_output_level(AG_SSC_PIN, LOW);
	
	/* Write address */
	ag_transfer(mod_address);
	
	/* Get response */
	receive = ag_transfer(0x00);
	
	gpio_pin_set_output_level(AG_SSC_PIN, HIGH);
	return receive;
}

/*
 *  Read a byte through SPI from the magnetometer registers.
 */
uint8_t m_read_byte(uint8_t address)
{
	uint8_t receive = 0x00;
	uint8_t mod_address = address | READ;
	
	gpio_pin_set_output_level(M_SSC_PIN, LOW);
	
	/* Write address */
	m_transfer(mod_address);
	
	/* Get response */
	receive = m_transfer(0x00);
	
	gpio_pin_set_output_level(M_SSC_PIN, HIGH);
	return receive;
}

/*
 * Read a specified number of bytes from the accelerometer/gyroscope
 * registers.
 */
void ag_read_bytes(uint8_t address, uint8_t* data, uint8_t count)
{
	/* Enable data transfer to device. */
	gpio_pin_set_output_level(AG_SSC_PIN, LOW);
	
	/* Send read request for register. */
	ag_transfer(READ | address);
	
	/* Read count number of bytes from the address. */
	for(int i = 0; i < count; i++) {
		data[i] = ag_transfer(0x00);
	}
	
	/* End data transfer to the device. */
	gpio_pin_set_output_level(AG_SSC_PIN, HIGH);
}

/*
 * Read a specified number of bytes from the magnetometer
 * registers.
 */
void m_read_bytes(uint8_t address, uint8_t* data, uint8_t count)
{
	/* Enable data transfer to device. */
	gpio_pin_set_output_level(M_SSC_PIN, LOW);
	
	if(count > 1){
		/* If count is greater than one modify the address to reflect that. */
		m_transfer(MULTIPLE_READ | address);
	} else {
		/* Modify the address to reflect a single byte read. */
		m_transfer(READ | address);
	}
	
	/* Read count number of bytes from the address. */
	for(int i = 0; i < count; i++) {
		data[i] = m_transfer(0x00);
	}
	
	/* End data transfer to the device. */
	gpio_pin_set_output_level(M_SSC_PIN, HIGH);
}

/*
 *  Writes a high value to the bit corresponding with uint8_t bit.
 *  The bits are ordered from right to left, zero indexed.
 */
uint8_t bit_write_high(uint8_t data, uint8_t bit)
{
	return data | (0b00000001 << bit);
}

/*
 *  Reads the bit value corresponding with uint8_t bit.
 *  This bits are ordered from right to left, zero indexed.
 */
uint8_t bit_read(uint8_t data, uint8_t bit)
{
	return data & (0b00000001 << bit);
}

/**************************************************************************************/
/***********  Accelerometer register initiation and other such nonsense. **************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for accelerometer output.
 */
void init_accelerometer() 
{
	/* No decimation		(00)  */
	/* All axis activated   (111) */
	/* Default zeros        (000) */
	ag_write(CTRL_REG5_XL, 0b00111000);
	
	/* Sample Rate   952hz  (110) */
	/* Scale		 8g     (11)  */
	/* Bandwidth	 408hz	(000) */
	ag_write(CTRL_REG6_XL, 0b11011000);
	
	/* High res off         (0)   */
	/* High res BW off		(00)  */
	/* Default zeros        (00)  */
	/* Filter bypass        (0)   */
	/* Default zero         (0)   */
	/* Filter off           (0)   */
	ag_write(CTRL_REG7_XL, 0b00000000);
}

/*
 * Read and print raw accelerometer data.
 */
void get_raw_accelerometer(uint16_t *output)
{
	uint8_t temp[6];
	
	/* Read data from all accelerometer output registers. */
	ag_read_bytes(OUT_X_L_XL, temp, 6);
	
	/* Store it into various variables. */
	output[0] = (temp[1] << 8) | temp[0]; /* X */
	output[1] = (temp[3] << 8) | temp[2]; /* Y */
	output[2] = (temp[5] << 8) | temp[4]; /* Z */
}

/**************************************************************************************/
/************  Gyroscope register initiation and other such nonsense. *****************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for gyroscope output.
 */
void init_gyroscope()
{
	/* Sample Rate   952hz  (110) */
	/* Scale         245dps (00)  */
	/* Default zero         (0)   */
	/* Bandwidth     33hz   (00)  */
	ag_write(CTRL_REG1_G, 0b11000000);
	
	/* Default zeros       (0000) */
	/* Interrupt select    (00)   */
	/* Output select       (00)   */
	ag_write(CTRL_REG2_G, 0b00000000);
	
	/* Low power off       (0)    */
	/* High pass off       (0)    */
	/* Default zeros       (00)   */
	/* High pass off       (000)  */
	ag_write(CTRL_REG3_G, 0b00000000);
	
	/* Default zeros       (00)   */
	/* Enable all axis     (111)  */
	/* Default zero        (0)    */
	/* Interrupts not used (00)   */
	ag_write(CTRL_REG4, 0b00111000);
	
	/* Default zeros       (00)   */
	/* Positive signs      (000)  */
	/* Orient              (000)  */
	ag_write(ORIENT_CFG_G, 0b00000000);
}

/*
 * Read and print raw gyroscope data.
 */
void get_raw_gyroscope(uint16_t* output)
{
	uint8_t temp[6];
	
	/* Read data from all accelerometer output registers. */
	ag_read_bytes(OUT_X_L_G, temp, 6);
	
	/* Store it into various variables. */
	output[0] = (temp[1] << 8) | temp[0]; /* X */
	output[1] = (temp[3] << 8) | temp[2]; /* Y */
	output[2] = (temp[5] << 8) | temp[4]; /* Z */
}

/**************************************************************************************/
/***********  Magnetometer register initiation and other such nonsense. ***************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for gyroscope output.
 */
void init_magnetometer()
{
	/* Temp comp disable  (0) */
	/* XY high pref      (11) */
	/* Output data 80hz (111) */
	/* Disable fast odr   (0) */
	/* Self test off      (0) */
	m_write(CTRL_REG1_M, 0b01111100);
	
	/* Default zero       (0) */
	/* Scale 4gauss      (00) */
	/* Default zero       (0) */
	/* Keep memory        (0) */
	/* Reset default      (0) */
	/* Default zeros     (00) */
	m_write(CTRL_REG2_M, 0b00000000);

	/* Disable I2C        (0) */
	/* Default zero       (0) */
	/* Low power mode off (0) */
	/* Default zeros     (00) */
	/* SPI read/write     (1) */
	/* Continuous conv   (00) */
	m_write(CTRL_REG3_M, 0b00000000);

	/* Default zeros   (0000) */
	/* Z High preform    (11) */
	/* Little endian      (0) */
	/* Default zero       (0) */
	m_write(CTRL_REG4_M, 0b00001100);

	/* Default zero       (0) */
	/* Update continuous  (0) */
	/* Default zeros (000000) */
	m_write(CTRL_REG5_M, 0b00000000);
}

/*
 * Read and print raw gyroscope data.
 */
void get_raw_magnetometer(uint16_t* output)
{
	uint8_t temp[6];
	
	/* Read data from all accelerometer output registers. */
	m_read_bytes(OUT_X_L_M, temp, 6);
	
	/* Store it into various variables. */
	output[0] = (temp[1] << 8) | temp[0]; /* X */
	output[1] = (temp[3] << 8) | temp[2]; /* Y */
	output[2] = (temp[5] << 8) | temp[4]; /* Z */
}

/**************************************************************************************/
/******************  Register initiation and other such nonsense. *********************/
/**************************************************************************************/

/*
 *  Configure UART console.
 */
void configure_console(struct uart_module* console_instance)
{
	struct uart_config config;
	
	uart_get_config_defaults(&config);
	config.baud_rate = 9600;
	
	stdio_serial_init(console_instance, UART0, &config);
	
	printf("Console configured\r\n");
}

/*
 *  Configure GPIO pins for SPI
 */
void configure_gpio(void)
{
	struct gpio_config config;
	gpio_get_config_defaults(&config);

	gpio_pinmux_cofiguration(AG_SSC_PIN, GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(AG_SCK_PIN, GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(AG_MISO_PIN, GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(AG_MOSI_PIN, GPIO_PINMUX_SEL_2);

	gpio_pinmux_cofiguration(M_SSC_PIN, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(M_SCK_PIN, GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(M_MISO_PIN, GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(M_MOSI_PIN, GPIO_PINMUX_SEL_2);

	
	/* Configure accelerometer/magnetometer Slave Select */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(AG_SSC_PIN, &config);
	
	/* Configure gyroscope Slave Select */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(M_SSC_PIN, &config);
	
	/* Configure Clock */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(AG_SCK_PIN, &config);
	
	/* Configure accelerometer/magnetometer MISO */
	config.direction = GPIO_PIN_DIR_INPUT;
	config.input_pull = GPIO_PIN_PULL_NONE;
	gpio_pin_set_config(AG_MISO_PIN, &config);
	
	/* Configure gyroscope MISO */
	config.direction = GPIO_PIN_DIR_INPUT;
	config.input_pull = GPIO_PIN_PULL_NONE;
	gpio_pin_set_config(M_MISO_PIN, &config);
	
	/* Configure MOSI */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(AG_MOSI_PIN, &config);
	
	/* Configure Button SW0 */
	config.direction = GPIO_PIN_DIR_INPUT;
	config.input_pull = GPIO_PIN_PULL_UP;
	gpio_pin_set_config(BUTTON_0_PIN, &config);

	gpio_pin_set_output_level(AG_SSC_PIN, HIGH);
	gpio_pin_set_output_level(AG_SSC_PIN, LOW);
	gpio_pin_set_output_level(AG_SSC_PIN, HIGH);
	
	gpio_pin_set_output_level(M_SSC_PIN, HIGH);
	gpio_pin_set_output_level(M_SSC_PIN, LOW);
	gpio_pin_set_output_level(M_SSC_PIN, HIGH);
}