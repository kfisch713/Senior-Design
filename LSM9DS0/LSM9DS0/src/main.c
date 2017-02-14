/*
   EXT1    SAMB      LSM9DS0   WIRE    English
   --------------------------------------------------------
   AM_SSC  EXT1 15   CSXM      Red     Slave Select
   G_SSC   EXT3 15   CSG       Red     Slave Select
   SCK     EXT1 18   SCL       Black   Serial Clock
   AM_MISO EXT1 17   SDOXM     Green   Master In Slave Out
   G_MISO  EXT3 17   SDOG      Green   Master In Slave Out
   MOSI    EXT1 16   SDA       Blue    Master Out Slave In
   VCC     EXT1 20   VIN       Red
   GND     EXT1 19   GND       Black
*/

#include <asf.h>
#include <LSM9DS0.h>
#include <BitBang.h>

struct uart_module console_instance;
struct spi_module master_instance;
struct spi_slave_inst slave_instance;

int main(void)
{	
	/* Board and console setup. */
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
	configure_console(&console_instance);
	
	/* Bit banged gpio configuration. */
	bb_configure_gpio();
	
	/* SPI configuration. */
	//configure_spi(&master_instance, &slave_instance);
	//configure_spi_master_callbacks();
	
	/* Start up SPI and read the status register. */
	uint8_t receive = 0x00;
	bb_am_read_byte(WHO_AM_I_XM);
	receive = bb_am_read_byte(WHO_AM_I_XM);
	printf("Should be 0x49: 0x%x\r\n", receive);
		
	/* Initialize accelerometer control registers. */
	bb_init_accelerometer();
	bb_init_accelerometer_odr(A_ODR_100);
	bb_init_accelerometer_scale(A_SCALE_2G);
	
	/* Initialize magnetometer control registers. */
	bb_init_magnetometer();
	bb_init_magnetometer_odr(M_ODR_100);
	bb_init_magnetometer_scale(M_SCALE_2GS);
	
	bb_init_gyroscope();
	bb_init_gyroscope_odr(G_ODR_190_BW_125);
	bb_init_gyroscope_scale(G_SCALE_2000DPS);

	while (1)
	{
		uint16_t accelerometer_data[3];
		
		bb_get_raw_accelerometer(accelerometer_data);
		
		printf("aX: %d, aY: %d, aZ: %d\r\n", accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);
		//bb_print_calculated_accelerometer(A_ODR_200, A_SCALE_6G);
		
		//uint8_t data = hw_spi_read(&master_instance, &slave_instance, WHO_AM_I_XM);
		//while (!transceive_complete) { /* Wait for write complete */ }
		//transceive_complete = false;
		//printf("Status register should be 0x49: 0x%x\r\n", data);
	}
}