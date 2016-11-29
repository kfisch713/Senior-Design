#include <asf.h>
#include "pwm.h"

static void configure_adc_pin3(void)
{
	struct adc_config config_adc;

	adc_get_config_defaults(&config_adc);

	config_adc.reference = ADC_REFERENCE_VBATT;
	config_adc.input_channel = ADC_INPUT_CH_GPIO_MS1;

	adc_init(&config_adc);

	adc_enable();
}

static void configure_adc_pin4(void)
{
	struct adc_config config_adc;

	adc_get_config_defaults(&config_adc);

	config_adc.reference = ADC_REFERENCE_VBATT;
	config_adc.input_channel = ADC_INPUT_CH_GPIO_MS2;

	adc_init(&config_adc);

	adc_enable();
}

static void configure_pwm_from_duty(char duty)
{
	struct pwm_config config_pwm;
	
	pwm_get_config_defaults(&config_pwm);
	
	config_pwm.duty_cycle = duty;
	config_pwm.pin_number_pad = PIN_LP_GPIO_4;
	config_pwm.pinmux_sel_pad = PINMUX_LP_GPIO_4_M_PWM0_OUT;
	
	pwm_init(PWM0, &config_pwm);
	pwm_enable(PWM0);
}

static void configure_pwm_from_duty_pin_11(char duty)
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

/*
int main(void)
{
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
	uint16_t result;
	uint16_t result2;
	configure_adc()_pin3;
	configure_adc_pin4();

	while (true) {
		do {
		} while (adc_read(ADC_INPUT_CH_GPIO_MS1, &result) == STATUS_BUSY);
		do {
		} while (adc_read(ADC_INPUT_CH_GPIO_MS2, &result2) == STATUS_BUSY);

		if (result > 1000)
			configure_pwm_from_duty(99);
		else
			configure_pwm_from_duty(result / 11);

		if (result2 > 1000)
			configure_pwm_from_duty_pin_11(99);
		else
			configure_pwm_from_duty_pin_11(result2 / 11);

		configure_adc();
		configure_adc_pin4();
	}
}
*/
