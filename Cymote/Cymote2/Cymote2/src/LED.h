/*
 * LED.h
 *
 * Created: 4/20/2017 9:10:47 AM
 *  Author: kfisc
 */ 


#ifndef LED_H_
#define LED_H_

#include "gpio.h"

#define RED_GPIO    PIN_LP_GPIO_22
#define GREEN_GPIO  PIN_LP_GPIO_3
#define BLUE_GPIO	PIN_LP_GPIO_4

void set_LED(bool red, bool green, bool blue);
void setup_LED(void);




#endif /* LED_H_ */