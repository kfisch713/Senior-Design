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

struct uart_module console_instance;
struct spi_module master_instance;
struct spi_slave_inst slave_instance;

int main(void)
{	
	/* Board and console setup. */
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
	configure_console(&console_instance);
	
	/* Bit banged gpio configuration. */
	configure_gpio();
	
	/* Start up SPI and read the status register. */
	uint8_t receive = 0x00;
	am_read_byte(WHO_AM_I_XM);
	receive = am_read_byte(WHO_AM_I_XM);
	printf("Should be 0x49: 0x%x\r\n", receive);
		
	/* Initialize accelerometer control registers. */
	init_accelerometer();
	init_accelerometer_odr(A_ODR_100);
	init_accelerometer_scale(A_SCALE_2G);
	
	/* Initialize magnetometer control registers. */
	init_magnetometer();
	init_magnetometer_odr(M_ODR_100);
	init_magnetometer_scale(M_SCALE_2GS);
	
	init_gyroscope();
	init_gyroscope_odr(G_ODR_190_BW_125);
	init_gyroscope_scale(G_SCALE_2000DPS);

	while (1)
	{
		uint16_t accelerometer_data[3];
		uint16_t magnetometer_data[3];
		
		get_raw_accelerometer(accelerometer_data);
		get_raw_magnetometer(magnetometer_data);
		
		//printf("aX: %5d\t aY: %5d\t aZ: %5d\r\n", accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);
		printf("aX: %5d\t aY: %5d\t aZ: %5d\t mX: %5d\t mY: %5d\t mZ: %5d\r\n", accelerometer_data[0], accelerometer_data[1], accelerometer_data[2], magnetometer_data[0], magnetometer_data[1], magnetometer_data[2]);
		//print_raw_magnetometer();
		//print_raw_gyroscope();
	}
}