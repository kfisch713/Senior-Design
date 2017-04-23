/*
 * ADC_GPIO.h
 *
 * Created: 4/22/2017 6:33:22 PM
 *  Author: kfisc
 */ 

 #include "LED.h"
 #include "Buttons.h"
 #include "adc_sam_b.h"

#ifndef ADC_GPIO_H_
#define ADC_GPIO_H_

void configure_adc_4(void);
void configure_adc_3(void);
void configure_gpio_pins(void);


#endif /* ADC_GPIO_H_ */