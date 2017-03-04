
#include "ADC_PWM.h"

void configure_adc_pin3(void)
{
	struct adc_config config_adc;

	adc_get_config_defaults(&config_adc);

	config_adc.reference = ADC_REFERENCE_VBATT;
	config_adc.input_channel = ADC_INPUT_CH_GPIO_MS1;

	adc_init(&config_adc);

	adc_enable();
}

void configure_adc_pin4(void)
{
	struct adc_config config_adc;

	adc_get_config_defaults(&config_adc);

	config_adc.reference = ADC_REFERENCE_VBATT;
	config_adc.input_channel = ADC_INPUT_CH_GPIO_MS2;

	adc_init(&config_adc);

	adc_enable();
}

void configure_pwm_from_duty_pin_5(char duty)
{
	// #define PIN_LP_GPIO_4 EXT1_PIN_5
	struct pwm_config config_pwm;

	pwm_get_config_defaults(&config_pwm);

	config_pwm.duty_cycle = duty;
	config_pwm.pin_number_pad = PIN_LP_GPIO_4;
	config_pwm.pinmux_sel_pad = PINMUX_LP_GPIO_4_M_PWM0_OUT;
	
	pwm_init(PWM0, &config_pwm);

	pwm_enable(PWM0);
}

void configure_pwm_from_duty_pin_10(char duty)
{
	// #define EXT1_PIN_10		PIN_LP_GPIO_5
	struct pwm_config config_pwm;

	pwm_get_config_defaults(&config_pwm);

	config_pwm.duty_cycle = duty;
	config_pwm.pin_number_pad = PIN_LP_GPIO_5;
	config_pwm.pinmux_sel_pad = PINMUX_LP_GPIO_5_M_PWM2_OUT;
	
	pwm_init(PWM2, &config_pwm);
	
	pwm_enable(PWM2);
}

void configure_pwm_from_duty_pin_11(char duty)
{
	// #define EXT1_PIN_11	 PIN_LP_GPIO_8
	struct pwm_config config_pwm;
	pwm_get_config_defaults(&config_pwm);

	config_pwm.duty_cycle = duty;
	config_pwm.pin_number_pad = PIN_LP_GPIO_8;
	config_pwm.pinmux_sel_pad = PINMUX_LP_GPIO_8_M_PWM1_OUT;

	pwm_init(PWM1, &config_pwm);

	pwm_enable(PWM1);
}