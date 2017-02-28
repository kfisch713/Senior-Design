/*
 * LSM9DS0.c
 *
 * Created: 11/19/2016 4:25:04 PM
 * Author: Michael Linthicum
 */ 

#include <asf.h>
#include <LSM9DS0.h>



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