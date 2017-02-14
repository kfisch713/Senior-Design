/*
 * LSM9DS0.c
 *
 * Created: 11/19/2016 4:25:04 PM
 * Author: Michael Linthicum
 */ 

#include <asf.h>
#include <LSM9DS0.h>

/*
 * Custom SPI read function.  Reads a single register (8 bits) from the 
 * LSM9DS0 sensor.
 */
uint8_t hw_spi_read(struct spi_module* master_instance, struct spi_slave_inst* slave_instance, uint8_t reg_to_read)
{
	uint8_t data = 0x00;
	uint8_t tx_data = reg_to_read | READ;
		
	/* Allow data transfer to device. */
	spi_select_slave(master_instance, slave_instance, true);
	
	spi_transceive_wait(master_instance, &tx_data, &data);
	
	/* End data transfer to device. */
	spi_select_slave(master_instance, slave_instance, false);
	
	return data;
}

/**************************************************************************************/
/******************  Register initiation and other such nonsense. *********************/
/**************************************************************************************/

/*
 * Configure the SPI hardware for the LSM9DS0
 */
void configure_spi(struct spi_module* master_instance, struct spi_slave_inst* slave_instance)
{
	struct spi_config config_spi_master;
	struct spi_slave_inst_config config_spi_slave;
	
	/* Initialize and attach SPI slave settings. */
	spi_slave_inst_get_config_defaults(&config_spi_slave);
	config_spi_slave.ss_pin = SLAVE_SELECT_PIN;
	spi_attach_slave(slave_instance, &config_spi_slave);
	
	/* Initialize the SPI master settings. */
	spi_get_config_defaults(&config_spi_master);
	config_spi_master.transfer_mode = SPI_TRANSFER_MODE_1;
	config_spi_master.data_order = SPI_DATA_ORDER_MSB;
	
	/* Not using SPI slave select hardware pins. */
	config_spi_master.pin_number_pad[2] = PINMUX_UNUSED;
	config_spi_master.pinmux_sel_pad[2] = PINMUX_UNUSED;
	
	spi_init(master_instance, SPI0, &config_spi_master);
	
	/* Begin SPI */
	spi_enable(master_instance);
	
	printf("SPI configured\r\n");
}

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