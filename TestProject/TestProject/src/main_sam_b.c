
#define DONT_USE_CMSIS_INIT
#include <asf.h>

void configure_console(struct uart_module* console_instance);

struct uart_module console_instance;

int main(void)
{
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
    configure_console(&console_instance);

    /* Replace with your application code */
    while (1) 
    {
		printf("Hello\r\n");
    }
}

/*
 *  Configure UART console.
 */
void configure_console(struct uart_module* console_instance)
{
	struct uart_config config;
	
	uart_get_config_defaults(&config);
	config.baud_rate = 115200;
	
	stdio_serial_init(console_instance, UART0, &config);
	
	printf("Console configured\r\n");
}