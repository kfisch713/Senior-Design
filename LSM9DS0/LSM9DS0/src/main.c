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

struct uart_module console_instance;

int main(void)
{
	/* Board, SPI, and console setup. */
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_3_25_MHZ);
	configure_console(&console_instance);
	configure_gpio();
	
	/* Start up SPI and read the status register. */
	uint8_t receive = 0x00;
	spi_read_byte(WHO_AM_I_XM);
	receive = spi_read_byte(WHO_AM_I_XM);
	printf("Should be 0x49: 0x%x\r\n", receive);
	
	/* Initialize accelerometer control registers. */
	init_accelerometer();
	init_accelerometer_odr(A_ODR_100);
	init_accelerometer_scale(A_SCALE_2G);

    while (1) 
    {
		print_raw_accelerometer();
		/* print_calculated_accelerometer(A_ODR_100, A_SCALE_2G); */
    }
}