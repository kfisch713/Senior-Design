/*
 * LSM9DS0.c
 *
 * Created: 11/19/2016 3:25:47 PM
 *  Author: Michael
 */ 

#include <asf.h>
#include <LSM9DS0.h>

uint8_t spi_transfer(uint8_t data) {
	uint8_t receive = 0x00;
	
	gpio_pin_set_output_level(SCK_PIN, HIGH);
	
	/* Send data to the device and receive data. */
	for(int i = 0; i < 8; i++) {
		gpio_pin_set_output_level(SCK_PIN, LOW);
		
		if(bit_read(data, 7-i) > 0x00) {
			gpio_pin_set_output_level(MOSI_PIN, HIGH);
			} else {
			gpio_pin_set_output_level(MOSI_PIN, LOW);
		}
		
		gpio_pin_set_output_level(SCK_PIN, HIGH);
		
		if(gpio_pin_get_input_level(MISO_PIN) == HIGH) {
			receive = bit_write_high(receive, 7-i);
		}
	}
	return receive;
}

/*
 *  Writes the value specified in uint8_t data to the register specified by
 *  uint8_t address.
 */
void my_spi_write(uint8_t address, uint8_t data) {
	gpio_pin_set_output_level(SSC_PIN, LOW);
	gpio_pin_set_output_level(SCK_PIN, HIGH);
	
	/* Write address */
	spi_transfer(address);
	
	/* Get response */
	spi_transfer(data);
	
	gpio_pin_set_output_level(SSC_PIN, HIGH);
	return;
}

/*
 *  Read a byte through SPI
 */
uint8_t spi_read_byte(uint8_t address) {
	uint8_t receive = 0x00;
	uint8_t mod_address = address | READ;
	
	gpio_pin_set_output_level(SSC_PIN, LOW);
	
	/* Write address */
	spi_transfer(mod_address);
	
	/* Get response */
	receive = spi_transfer(0x00);
	
	gpio_pin_set_output_level(SSC_PIN, HIGH);
	return receive;
}

/*
 * Read a specified number of bytes from the device.
 */
void spi_read_bytes(uint8_t address, uint8_t* data, uint8_t count)
{
	/* Enable data transfer to device. */
	gpio_pin_set_output_level(SSC_PIN, LOW);
	
	if(count > 1){
		/* If count is greater than one modify the address to reflect that. */
		spi_transfer(MULTIPLE_READ | address);
	} else {
		/* Modify the address to reflect a single byte read. */
		spi_transfer(READ | address);
	}
	
	/* Read count number of bytes from the address. */
	for(int i = 0; i < count; i++) {
		data[i] = spi_transfer(0x00);
	}
	
	/* End data transfer to the device. */
	gpio_pin_set_output_level(SSC_PIN, HIGH);
}

/*
 * Read and print raw accelerometer data.
 */
void print_raw_accelerometer()
{
	uint8_t temp[6];
	
	/* Read data from all accelerometer output registers. */
	spi_read_bytes(OUT_X_L_A, temp, 6);
	
	/* Store it into various variables. */
	uint16_t ax = (temp[1] << 8) | temp[0];
	uint16_t ay = (temp[3] << 8) | temp[2];
	uint16_t az = (temp[5] << 8) | temp[4];
	
	printf("X: %d, Y: %d, Z: %d\r\n", ax, ay, az);
}

/*
 * Read, print, and calculate accelerometer data.
 */
void print_calculated_accelerometer(a_odr rate, a_scale scale)
{
	uint8_t temp[6];
	float a_res;
	
	if(scale == A_SCALE_16G) {
		a_res = 16.0 / 32768.0;
	} else {
		a_res = (((float) scale + 1.0) * 2.0) / 32768.0;
	}
	
	/* Read data from all accelerometer output registers. */
	spi_read_bytes(OUT_X_L_A, temp, 6);

	/* Store it into various variables. */
	int ax = (temp[1] << 8) | temp[0];
	int ay = (temp[3] << 8) | temp[2];
	int az = (temp[5] << 8) | temp[4];

	float ax_calc = a_res * (float) ax;
	float ay_calc = a_res * (float) ay;
	float az_calc = a_res * (float) az;
	
	/* Print that shit. */
	printf("X: %f, Y: %f, Z: %f\r\n", ax_calc, ay_calc, az_calc);
}

/*
 *  Writes a high value to the bit corresponding with uint8_t bit.
 *  The bits are ordered from right to left, zero indexed.
 */
uint8_t bit_write_high(uint8_t data, uint8_t bit) {
	return data | (0b00000001 << bit);
}

/*
 *  Reads the bit value corresponding with uint8_t bit.
 *  This bits are ordered from right to left, zero indexed.
 */
uint8_t bit_read(uint8_t data, uint8_t bit) {
	return data & (0b00000001 << bit);
}

/**************************************************************************************/
/******************  Register initiation and other such nonsense. *********************/
/**************************************************************************************/

/*
 * Initialize data transfer registers for accelerometer output.
 */
void init_accelerometer() {
	my_spi_write(CTRL_REG0_XM, 0x00);
	my_spi_write(CTRL_REG1_XM, 0x57);
	my_spi_write(CTRL_REG2_XM, 0x00);
	my_spi_write(CTRL_REG3_XM, 0x04);
}

/*
 * Set the output data rate for the accelerometer.
 */
void init_accelerometer_odr(a_odr rate) {
	
	/* Read the current CTRL_REG1_XM value. */
	uint8_t temp = spi_read_byte(CTRL_REG1_XM);

	/* Mask the current odr bits. */
	temp &= 0xFF ^ (0xF << 4);

	/* Shift in wanted odr bits. */
	temp |= (rate << 4);

	/* Write back to CTRL_REG1_XM register. */
	my_spi_write(CTRL_REG1_XM, temp);
}

/*
 * Set the scale for the data from the accelerometer.
 */
void init_accelerometer_scale(a_scale scale) {
	/* Read the current CTRL_REG2_XM value. */
	uint8_t temp = spi_read_byte(CTRL_REG2_XM);

	/* Mask the current scale bits. */
	temp &= 0xFF ^ (0x3 << 3);

	/* Shift in wanted scale bits. */
	temp |= (scale << 3);

	/* Write back to the CTRL_REG2_XM register. */
	my_spi_write(CTRL_REG2_XM, temp);
}

/***********************************************************************/
/*************** Board and Console set up nonsense *********************/
/***********************************************************************/

/*
 *  Configure UART console.
 */
void configure_console(struct uart_module* console_instance)
{
	struct uart_config config;
	
	uart_get_config_defaults(&config);
	config.baud_rate = 9600;
	
	stdio_serial_init(console_instance, UART0, &config);
	
	puts("Console configured\r");
}

/*
 *  Configure GPIO pins for SPI
 */
void configure_gpio(void)
{
	struct gpio_config config;
	gpio_get_config_defaults(&config);
	
	/* Configure Slave Select */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(SSC_PIN, &config);
	
	/* Configure Clock */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(SCK_PIN, &config);
	
	/* Configure MISO */
	config.direction = GPIO_PIN_DIR_INPUT;
	config.input_pull = GPIO_PIN_PULL_NONE;
	gpio_pin_set_config(MISO_PIN, &config);
	
	/* Configure MOSI */
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(MOSI_PIN, &config);
	
	/* Configure Button SW0 */
	config.direction = GPIO_PIN_DIR_INPUT;
	config.input_pull = GPIO_PIN_PULL_UP;
	gpio_pin_set_config(BUTTON_0_PIN, &config);
	
	gpio_pin_set_output_level(SSC_PIN, HIGH);
	gpio_pin_set_output_level(SSC_PIN, LOW);
	gpio_pin_set_output_level(SSC_PIN, HIGH);
	
	
	puts("GPIO configured\r");
}