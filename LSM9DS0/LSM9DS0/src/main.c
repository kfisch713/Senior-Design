/*
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
#include <LSM9DS0.h>

static void configure_spi_master(void);

struct uart_module console_instance;
struct spi_module spi_master_instance;
struct spi_slave_inst slave;
volatile bool transrev_complete_spi_master = false;

int main(void)
{
	/* Board, SPI, and console setup. */
	//system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_3_25_MHZ);
	configure_console(&console_instance);
	//configure_gpio();
	
	while(1) {
		printf("Hello\r\n");
	}
}

static void configure_spi_master(void) {
	struct spi_config config_spi_master;
	struct spi_slave_inst_config slave_dev_config;
	
	/* Configure and initialize software device instance of peripheral slave */
	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = SSC_PIN;
	spi_attach_slave(&slave, &slave_dev_config);
	
	/* Configure, initialize and enable SPI module */
	spi_get_config_defaults(&config_spi_master);
	config_spi_master.clock_divider = 154;
	config_spi_master.transfer_mode = SPI_TRANSFER_MODE_1;
	config_spi_master.data_order = SPI_DATA_ORDER_MSB;
	
	/* Configure Slave Select */
	config_spi_master.pin_number_pad[2] = PINMUX_UNUSED;
	config_spi_master.pinmux_sel_pad[2] = PINMUX_UNUSED;
	
	spi_init(&spi_master_instance, SPI0, &config_spi_master);
	spi_enable(&spi_master_instance);
}