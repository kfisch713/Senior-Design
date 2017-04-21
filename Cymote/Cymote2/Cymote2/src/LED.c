/*
 * LED.c
 *
 * Created: 4/20/2017 9:10:32 AM
 *  Author: kfisc
 */ 

 #include "LED.h"
 /* LED pins are active low */
 void set_LED(bool red, bool green, bool blue){

	 (red) ? gpio_pin_set_output_level(RED_GPIO, LOW) : gpio_pin_set_output_level(RED_GPIO, HIGH);
	 (green) ? gpio_pin_set_output_level(GREEN_GPIO, LOW) : gpio_pin_set_output_level(GREEN_GPIO, HIGH);
	 (blue) ? gpio_pin_set_output_level(BLUE_GPIO, LOW) : gpio_pin_set_output_level(BLUE_GPIO, HIGH);

 }

 void setup_LED(void){
	struct gpio_config config;
	gpio_get_config_defaults(&config);

	//USE MUX0 so these gpio just act as logic 1 and 0;
	gpio_pinmux_cofiguration(RED_GPIO, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(GREEN_GPIO, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(BLUE_GPIO, GPIO_PINMUX_SEL_0);

	//setup pins as output
	config.direction = GPIO_PIN_DIR_OUTPUT;
	config.input_pull = GPIO_PIN_PULL_NONE;
	gpio_pin_set_config(RED_GPIO, &config);
	gpio_pin_set_config(GREEN_GPIO, &config);
	gpio_pin_set_config(BLUE_GPIO, &config);

	//default value
	gpio_pin_set_output_level(RED_GPIO, LOW);
	gpio_pin_set_output_level(GREEN_GPIO, HIGH);
	gpio_pin_set_output_level(BLUE_GPIO, HIGH);
 }