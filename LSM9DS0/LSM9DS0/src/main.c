/**
   EXT1  SAMB   LSM9DS0   WIRE    English
   ---------------------------------------
   SSC   15     CSXM      Red     Slave Select
   SCK   18     SCL       Black   Serial Clock
   MISO  17     SDOXM     Green   Master In Slave Out
   MOSI  16     SDA       Blue    Master Out Slave In
   VCC   20     VIN       Red
   GND   19     GND       Black
*/
#include <asf.h>

#define SLAVE_SELECT_PIN  PIN_LP_GPIO_8

#define READ              0x80
#define WHO_AM_I          0x0F
#define CTRL_REG0_XM      0x1F
#define CTRL_REG1_XM      0x20
#define CTRL_REG2_XM      0x21
#define CTRL_REG3_XM      0x22
#define FIFO_CTRL_REG     0x2E
#define OUT_X_L_A         0x28
#define OUT_X_H_A         0x29
#define OUT_Y_L_A         0x2A
#define OUT_Y_H_A         0x2B
#define OUT_Z_L_A         0x2C
#define OUT_Z_H_A         0x2D

uint8_t my_spi_read(uint8_t reg_to_read);
void my_spi_write(uint8_t reg_to_write, uint8_t data);
void configure_console(void);
void configure_spi(void);
void configure_gpio(void);
void configure_xm_ctrl_regs(void);

struct spi_module master_instance;
struct spi_slave_inst slave_instance;
struct uart_module console_instance;

int main(void)
{	
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
	
	configure_console();
	configure_gpio();
	configure_spi();
	
	//configure_xm_ctrl_regs();

	for(uint32_t i = 0; i < 0x000DFFFF; i++);
	puts("\r");
	
	printf("Register read the first time: %x\n\r", my_spi_read(WHO_AM_I));
	
	puts("\r");
	
    while(true) 
    {
		uint16_t x_data_16 = 0x0000;
		uint8_t x_data_8 = 0x00;
		
		uint16_t y_data_16 = 0x0000;
		uint8_t y_data_8 = 0x00;
		
		uint16_t z_data_16 = 0x0000;
		uint8_t z_data_8 = 0x00;
		
		x_data_16 = my_spi_read(OUT_X_H_A);
		x_data_8 = my_spi_read(OUT_X_L_A);
		x_data_16 << 8;
		x_data_16 |= x_data_8;
		
		
		y_data_16 = my_spi_read(OUT_Y_H_A);
		y_data_8 = my_spi_read(OUT_Y_L_A);
		y_data_16 << 8;
		y_data_16 |= y_data_8;
		
		z_data_16 = my_spi_read(OUT_Z_H_A);
		z_data_8 = my_spi_read(OUT_Z_L_A);
		z_data_16 << 8;
		z_data_16 |= z_data_8;
		
		
		printf("X: %d Y: %d Z: %d\r\n", x_data_16, y_data_16, z_data_16);
    }
}

uint8_t my_spi_read(uint8_t reg_to_read)
{	
	uint8_t* data = NULL;
	gpio_pin_set_output_level(SLAVE_SELECT_PIN, LOW);
			
	// Select the register to write to
	spi_write(&master_instance, reg_to_read | READ);
	for(uint32_t i = 0; i < 0x0001FFFF; i++){/* Wait for it to write */}
	spi_read(&master_instance, data);
	
	//gpio_pin_set_output_level(SLAVE_SELECT_PIN, HIGH);
	
	return *data;
}

void my_spi_write(uint8_t reg_to_write, uint8_t data)
{
	//spi_select_slave(&master_instance, &slave_instance, true);
	gpio_pin_set_output_level(SLAVE_SELECT_PIN, LOW);
	
	// Select the register to write to
	spi_write(&master_instance, reg_to_write);
	for(uint32_t i = 0; i < 0x000DFFFF; i++);
	spi_write(&master_instance, data);
	
	//spi_select_slave(&master_instance, &slave_instance, false);
	//gpio_pin_set_output_level(SLAVE_SELECT_PIN, HIGH);
}

/**
 *  Configure UART console.
 */
void configure_console(void)
{
	struct uart_config config;
	
	uart_get_config_defaults(&config);
	config.baud_rate = 115200;
	
	stdio_serial_init(&console_instance, UART0, &config);
	
	puts("Console configured\r");
}


void configure_gpio(void)
{
	struct gpio_config config;
	gpio_get_config_defaults(&config);
	
	// Configure Slave Select
	config.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(SLAVE_SELECT_PIN, &config);
	
	// Configure Button SW0
	config.direction = GPIO_PIN_DIR_INPUT;
	config.input_pull = GPIO_PIN_PULL_UP;
	gpio_pin_set_config(BUTTON_0_PIN, &config);
	
	puts("GPIO configured\r");
}

void configure_spi(void)
{
	struct spi_config config_spi_master;
	struct spi_slave_inst_config config_spi_slave;
	
	/* Initialize the peripheral (slave) */
	spi_slave_inst_get_config_defaults(&config_spi_slave);
	spi_attach_slave(&slave_instance, &config_spi_slave);
	
	/* Initialize the MCU (master) */
	spi_get_config_defaults(&config_spi_master);
	config_spi_master.transfer_mode = SPI_TRANSFER_MODE_1;
	config_spi_master.clock_divider = 12;
	
	//config_spi_master.pin_number_pad[2] = PINMUX_UNUSED;
	//config_spi_master.pinmux_sel_pad[2] = PINMUX_UNUSED;
	
	spi_init(&master_instance, SPI0, &config_spi_master);
	
	/* Begin SPI */
	spi_enable(&master_instance);
	
	puts("SPI configured\r");
}

void configure_xm_ctrl_regs(void)
{
	my_spi_write(CTRL_REG0_XM, 0b00000000);
	my_spi_write(CTRL_REG1_XM, 0b01100111);
	my_spi_write(CTRL_REG2_XM, 0b00000000);
	my_spi_write(CTRL_REG3_XM, 0b00000100);
	
	my_spi_write(FIFO_CTRL_REG, 0b00111111);
	
	puts("XM Control registers configured\r");
}