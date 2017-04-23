/*
 * Buttons.c
 *
 * Created: 4/23/2017 11:47:19 AM
 *  Author: kfisc
 */ 

 #include "Buttons.h"

void get_button_data(bool* button1, bool* button2, bool* button3, bool* button4){
	*button1 = gpio_pin_get_input_level(BUTTON1);
	*button2 = gpio_pin_get_input_level(BUTTON2);
	*button3 = gpio_pin_get_input_level(BUTTON3);
	*button4 = gpio_pin_get_input_level(BUTTON4);
}