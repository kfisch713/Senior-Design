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

/*
	Top to Bottom
	CS_AG	White	EXT1 15
	MOSI	Blue	EXT1 16
	MISO	Green	EXT1 17
	SCLK	Purple	EXT1 18
	GND		Gray	EXT1 19
	VCC		Orange	EXT1 20
*/

#include <asf.h>
#include <LSM9DS1.h>

struct uart_module console_instance;

int main(void)
{	
	/* Board and console setup. */
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
	configure_console(&console_instance);
	
	/* Bit banged gpio configuration. */
	configure_gpio();
	
	/* Start up SPI and read the status register. */
	uint8_t receive = ag_read_byte(WHO_AM_I_XM);
	printf("Should be 0x68: 0x%x\r\n", receive);
		
	/* Initialize accelerometer control registers.*/
	//init_accelerometer();
	
	/* Initialize gyroscope control registers. */
	//init_gyroscope();
	
	/* Initialize magnetometer control registers. */
	init_magnetometer();

	while (1)
	{
		//uint16_t accelerometer_data[3];
		//get_raw_accelerometer(accelerometer_data);
		//printf("aX: %5d\t aY: %5d\t aZ: %5d\r\n", accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);
		
		//uint16_t gyroscope_data[3];
		//get_raw_gyroscope(gyroscope_data);
		//printf("gX: %5d\t gY: %5d\t gZ: %5d\r\n", gyroscope_data[0], gyroscope_data[1], gyroscope_data[2]);
	
		uint16_t magnetometer_data[3];
		get_raw_magnetometer(magnetometer_data);
		printf("mX: %5d\t mY: %5d\t mZ: %5d\r\n", magnetometer_data[0], magnetometer_data[1], magnetometer_data[2]);
	}
}