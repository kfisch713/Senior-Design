/*
 * Buttons.h
 *
 * Created: 4/22/2017 6:26:19 PM
 *  Author: kfisc
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "cymote_selector.h"
#include <stdbool.h>
#include "adc_sam_b.h"
#include "gpio.h"

//Buttons are only correct for Alpha model 
#if ALPHA_CONNECTIONS
#define BUTTON1 PIN_AO_GPIO_2
#define BUTTON2 PIN_AO_GPIO_0
#define BUTTON3 PIN_LP_GPIO_9
#define BUTTON4 PIN_LP_GPIO_20
#endif

void get_button_data(bool* button1, bool* button2, bool* button3, bool* button4);

#endif /* BUTTONS_H_ */