/*
 * LSM9DS0.c
 *
 * Created: 1/23/2017 5:51:07 PM
 * Author: Michael Linthicum
 *	       mpl@iastate.edu
 */ 

#include <asf.h>
#include <LSM9DS0.h>

uint8_t am_transfer(uint8_t data) {
	uint8_t receive = 0x00;
	
	gpio_pin_set_output_level(AM_SCK_PIN, HIGH);
	
	/* Send data to the device and receive data. */
	for(int i = 0; i < 8; i++) {
		gpio_pin_set_output_level(AM_SCK_PIN, LOW);
		
		if(bit_read(data, 7-i) > 0x00) {
			gpio_pin_set_output_level(AM_MOSI_PIN, HIGH);
			} else {
			gpio_pin_set_output_level(AM_MOSI_PIN, LOW);
		}
		
		gpio_pin_set_output_level(AM_SCK_PIN, HIGH);
		
		if(gpio_pin_get_input_level(AM_MISO_PIN) == HIGH) {
			receive = bit_write_high(receive, 7-i);
		}
	}
	return receive;
}

uint8_t g_transfer(uint8_t data) {
	uint8_t receive = 0x00;
	
	gpio_pin_set_output_level(G_SCK_PIN, HIGH);
	
	/* Send data to the device and receive data. */
	for(int i = 0; i < 8; i++) {
		gpio_pin_set_output_level(G_SCK_PIN, LOW);
		
		if(bit_read(data, 7-i) > 0x00) {
			gpio_pin_set_output_level(G_MOSI_PIN, HIGH);
			} else {
			gpio_pin_set_output_level(G_MOSI_PIN, LOW);
		}
		
		gpio_pin_set_output_level(G_SCK_PIN, HIGH);
		
		if(gpio_pin_get_input_level(G_MISO_PIN) == HIGH) {
			receive = bit_write_high(receive, 7-i);
		}
	}
	return receive;
}

/*
 *  Writes the value specified in uint8_t data to the accelerometer/magnetometer 
 *  register specified by uint8_t address.
 */
void am_write(uint8_t address, uint8_t data)
{
	gpio_pin_set_output_level(AM_SSC_PIN, LOW);
	gpio_pin_set_output_level(AM_SCK_PIN, HIGH);
	
	/* Write address */
	am_transfer(address);
	
	/* Get response */
	am_transfer(data);
	
	gpio_pin_set_output_level(AM_SSC_PIN, HIGH);
	return;
}

/*
 *  Writes the value specified in uint8_t data to the gyroscope register 
 *  specified by uint8_t address.
 */
void g_write(uint8_t address, uint8_t data)
{
	gpio_pin_set_output_level(G_SSC_PIN, LOW);
	gpio_pin_set_output_level(G_SCK_PIN, HIGH);
	
	/* Write address. */
	g_transfer(address);
	
	/* Get response. */
	g_transfer(data);
	
	gpio_pin_set_output_level(G_SSC_PIN, HIGH);
	return;
}

/*
 *  Read a byte through SPI from the accelerometer/magnetometer registers.
 */
uint8_t am_read_byte(uint8_t address)
{
	uint8_t receive = 0x00;
	uint8_t mod_address = address | READ;
	
	gpio_pin_set_output_level(AM_SSC_PIN, LOW);
	
	/* Write address */
	am_transfer(mod_address);
	
	/* Get response */
	receive = am_transfer(0x00);
	
	gpio_pin_set_output_level(AM_SSC_PIN, HIGH);
	return receive;
}

/*
 *  Read a byte through SPI from the gyroscope registers.
 */
uint8_t g_read_byte(uint8_t address)
{
	uint8_t receive = 0x00;
	uint8_t mod_address = address | READ;
	
	gpio_pin_set_output_level(G_SSC_PIN, LOW);
	
	/* Write address */
	g_transfer(mod_address);
	
	/* Get response */
	receive = g_transfer(0x00);
	
	gpio_pin_set_output_level(G_SSC_PIN, HIGH);
	return receive;
}

/*
 * Read a specified number of bytes from the accelerometer/magnetometer
 * registers.
 */
void am_read_bytes(uint8_t address, uint8_t* data, uint8_t count)
{
	/* Enable data transfer to device. */
	gpio_pin_set_output_level(AM_SSC_PIN, LOW);
	
	if(count > 1){
		/* If count is greater than one modify the address to reflect that. */
		am_transfer(MULTIPLE_READ | address);
	} else {
		/* Modify the address to reflect a single byte read. */
		am_transfer(READ | address);
	}
	
	/* Read count number of bytes from the address. */
	for(int i = 0; i < count; i++) {
		data[i] = am_transfer(0x00);
	}
	
	/* End data transfer to the device. */
	gpio_pin_set_output_level(AM_SSC_PIN, HIGH);
}

/*
 * Read a specified number of bytes from the gyroscope
 * registers.
 */
void g_read_bytes(uint8_t address, uint8_t* data, uint8_t count)
{
	/* Enable data transfer to device. */
	gpio_pin_set_output_level(G_SSC_PIN, LOW);
	
	if(count > 1){
		/* If count is greater than one modify the address to reflect that. */
		g_transfer(MULTIPLE_READ | address);
	} else {
		/* Modify the address to reflect a single byte read. */
		g_transfer(READ | address);
	}
	
	/* Read count number of bytes from the address. */
	for(int i = 0; i < count; i++) {
		data[i] = g_transfer(0x00);
	}
	
	/* End data transfer to the device. */
	gpio_pin_set_output_level(G_SSC_PIN, HIGH);
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
	am_write(CTRL_REG0_XM, 0x00);
	am_write(CTRL_REG1_XM, 0x57);
	am_write(CTRL_REG2_XM, 0x00);
	am_write(CTRL_REG3_XM, 0x04);
}

/*
 * Set the output data rate for the accelerometer.
 */
void init_accelerometer_odr(a_odr rate)
{
	/* Read the current CTRL_REG1_XM value. */
	uint8_t temp = am_read_byte(CTRL_REG1_XM);

	/* Mask the current odr bits. */
	temp &= 0xFF ^ (0xF << 4);

	/* Shift in wanted odr bits. */
	temp |= (rate << 4);

	/* Write back to CTRL_REG1_XM register. */
	am_write(CTRL_REG1_XM, temp);
}

/*
 * Set the scale for the data from the accelerometer.
 */
void init_accelerometer_scale(a_scale scale)
{
	/* Read the current CTRL_REG2_XM value. */
	uint8_t temp = am_read_byte(CTRL_REG2_XM);

	/* Mask the current scale bits. */
	temp &= 0xFF ^ (0x3 << 3);

	/* Shift in wanted scale bits. */
	temp |= (scale << 3);

	/* Write back to the CTRL_REG2_XM register. */
	am_write(CTRL_REG2_XM, temp);
}

/*
 * Read and print raw accelerometer data.
 */
void get_raw_accelerometer(uint16_t *output)
{
	uint8_t temp[6];
	
	/* Read data from all accelerometer output registers. */
	am_read_bytes(OUT_X_L_A, temp, 6);
	
	/* Store it into various variables. */
	output[0] = (temp[1] << 8) | temp[0]; /* X */
	output[1] = (temp[3] << 8) | temp[2]; /* Y */
	output[2] = (temp[5] << 8) | temp[4]; /* Z */
}

/*
 * Read, print, and calculate accelerometer data.
 */
void print_calculated_accelerometer(a_odr rate, a_scale scale)
{
	uint8_t temp[6];
	long a_res;
	
	if(scale == A_SCALE_16G) {
		a_res = (16 * 10000000L) / 32768;
	} else {
		a_res = (((scale + 1) * 2) * 10000000L) / 32768;
	}
	
	/* Read data from all accelerometer output registers. */
	am_read_bytes(OUT_X_L_A, temp, 6);

	/* Store it into various variables. */
	int ax = (temp[1] << 8) | temp[0];
	int ay = (temp[3] << 8) | temp[2];
	int az = (temp[5] << 8) | temp[4];

	long ax_calc = a_res * ax;
	long ay_calc = a_res * ay;
	long az_calc = a_res * az;
	
	/* Print that shit. */
	printf("X: %ld.%ld, Y: %ld.%ld, Z: %ld.%ld\r\n", ax_calc/10000000L, ax_calc%10000000L, ay_calc/10000000L, ay_calc%10000000L, az_calc/10000000L, az_calc%10000000L);
}

/**************************************************************************************/
/***********  Magnetometer register initiation and other such nonsense. ***************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for magnetometer output.
 */
void init_magnetometer()
{
	am_write(CTRL_REG5_XM, 0x94);
	am_write(CTRL_REG6_XM, 0x00);
	am_write(CTRL_REG7_XM, 0x00);
	am_write(CTRL_REG4_XM, 0x04);
	am_write(INT_CTRL_REG_M, 0x09);
}

/*
 * Set the output data rate for the magnetometer.
 */
void init_magnetometer_odr(m_odr rate)
{
	/* Read the current CTRL_REG5_XM value. */
	uint8_t temp = am_read_byte(CTRL_REG5_XM);

	/* Mask the current odr bits. */
	temp &= 0xFF ^ (0x7 << 2);

	/* Shift in wanted odr bits. */
	temp |= (rate << 2);

	/* Write back to CTRL_REG5_XM register. */
	am_write(CTRL_REG5_XM, temp);
}

/*
 * Set the scale for the data from the magnetometer.
 */
void init_magnetometer_scale(m_scale scale)
{
	/* Read the current CTRL_REG6_XM value. */
	uint8_t temp = am_read_byte(CTRL_REG6_XM);

	/* Mask the current scale bits. */
	temp &= 0xFF ^ (0x3 << 5);

	/* Shift in wanted scale bits. */
	temp |= (scale << 5);

	/* Write back to the CTRL_REG6_XM register. */
	am_write(CTRL_REG6_XM, temp);
}

/*
 * Read and print raw magnetometer data.
 */
void get_raw_magnetometer(uint16_t *output)
{
	uint8_t temp[6];
	
	/* Read data from all accelerometer output registers. */
	am_read_bytes(OUT_X_L_M, temp, 6);
	
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
void init_gyroscope()
{
	g_write(CTRL_REG1_G, 0x0F);
	g_write(CTRL_REG2_G, 0x00);
	g_write(CTRL_REG3_G, 0x88);
	g_write(CTRL_REG4_G, 0x00);
	g_write(CTRL_REG5_G, 0x00);
}

/*
 * Set the output data rate for the gyroscope.
 */
void init_gyroscope_odr(g_odr rate)
{
	/* Read the current CTRL_REG5_XM value. */
	uint8_t temp = g_read_byte(CTRL_REG1_G);

	/* Mask the current odr bits. */
	temp &= 0xFF ^ (0xF << 4);

	/* Shift in wanted odr bits. */
	temp |= (rate << 4);

	/* Write back to CTRL_REG5_XM register. */
	g_write(CTRL_REG1_G, temp);
}

/*
 * Set the scale for the data from the gyroscope.
 */
void init_gyroscope_scale(g_scale scale)
{
	/* Read the current CTRL_REG6_XM value. */
	uint8_t temp = g_read_byte(CTRL_REG4_G);

	/* Mask the current scale bits. */
	temp &= 0xFF ^ (0x3 << 4);

	/* Shift in wanted scale bits. */
	temp |= (scale << 4);

	/* Write back to the CTRL_REG6_XM register. */
	g_write(CTRL_REG4_G, temp);
}

/*
 * Read and print raw gyroscope data.
 */
void print_raw_gyroscope()
{
	uint8_t temp[6];
	
	/* Read data from all accelerometer output registers. */
	g_read_bytes(OUT_X_L_G, temp, 6);
	
	/* Store it into various variables. */
	uint16_t gx = (temp[1] << 8) | temp[0];
	uint16_t gy = (temp[3] << 8) | temp[2];
	uint16_t gz = (temp[5] << 8) | temp[4];
	
	printf("gX: %d\t gY: %d\t gZ: %d\r\n", gx, gy, gz);
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
	
	/* Configure accelerometer/magnetometer Slave Select */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(AM_SSC_PIN, &config);
	
	/* Configure gyroscope Slave Select */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(G_SSC_PIN, &config);
	
	/* Configure Clock */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(AM_SCK_PIN, &config);
	
	/* Configure accelerometer/magnetometer MISO */
	config.direction = GPIO_PIN_DIR_INPUT;
	config.input_pull = GPIO_PIN_PULL_NONE;
	gpio_pin_set_config(AM_MISO_PIN, &config);
	
	/* Configure gyroscope MISO */
	config.direction = GPIO_PIN_DIR_INPUT;
	config.input_pull = GPIO_PIN_PULL_NONE;
	gpio_pin_set_config(G_MISO_PIN, &config);
	
	/* Configure MOSI */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(AM_MOSI_PIN, &config);
	
	/* Configure Button SW0 */
	config.direction = GPIO_PIN_DIR_INPUT;
	config.input_pull = GPIO_PIN_PULL_UP;
	gpio_pin_set_config(BUTTON_0_PIN, &config);
	
	gpio_pin_set_output_level(AM_SSC_PIN, HIGH);
	gpio_pin_set_output_level(AM_SSC_PIN, LOW);
	gpio_pin_set_output_level(AM_SSC_PIN, HIGH);
	
	gpio_pin_set_output_level(G_SSC_PIN, HIGH);
	gpio_pin_set_output_level(G_SSC_PIN, LOW);
	gpio_pin_set_output_level(G_SSC_PIN, HIGH);
	
	
	puts("GPIO configured\r");
}
