/*
 * ADC_PWM.h
 *
 * Created: 11/28/2016 9:48:52 PM
 *  Author: dshauger
 */ 


#ifndef ADC_PWM_H_
#define ADC_PWM_H_

#include "pwm.h"
#include "adc_sam_b.h"

void configure_adc_pin3(void);

void configure_adc_pin4(void);

void configure_pwm_from_duty_pin_5(char duty);

void configure_pwm_from_duty_pin_10(char duty);

void configure_pwm_from_duty_pin_11(char duty);

#endif /* ADC_PWM_H_ */