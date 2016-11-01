#include <asf.h>

static void configure_pwm_from_duty_pin_5(char duty)
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

static void configure_pwm_from_duty_pin_10(char duty)
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

int main(void)
{
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
	int i = 0;

	while(1){
		// Red connected to pin 5
		// Green connected to pin 10
		// Blue connected to pin 11

		// Red
		configure_pwm_from_duty_pin_5(99);
		configure_pwm_from_duty_pin_10(0);
		configure_pwm_from_duty_pin_11(0);
		i = 1000000;
		while(i >= 0) i--;

		// Orange
		configure_pwm_from_duty_pin_5(99);
		configure_pwm_from_duty_pin_10(20);
		configure_pwm_from_duty_pin_11(0);
		i = 1000000;
		while(i >= 0) i--;

		// Yellow
		configure_pwm_from_duty_pin_5(99);
		configure_pwm_from_duty_pin_10(99);
		configure_pwm_from_duty_pin_11(0);
		i = 1000000;
		while(i >= 0) i--;

		// Green
		configure_pwm_from_duty_pin_5(0);
		configure_pwm_from_duty_pin_10(99);
		configure_pwm_from_duty_pin_11(0);
		i = 1000000;
		while(i >= 0) i--;

		// Blue
		configure_pwm_from_duty_pin_5(0);
		configure_pwm_from_duty_pin_10(0);
		configure_pwm_from_duty_pin_11(99);
		i = 1000000;
		while(i >= 0) i--;

		// Purple
		configure_pwm_from_duty_pin_5(99);
		configure_pwm_from_duty_pin_10(0);
		configure_pwm_from_duty_pin_11(99);
		i = 1000000;
		while(i >= 0) i--;

		// White
		configure_pwm_from_duty_pin_5(99);
		configure_pwm_from_duty_pin_10(99);
		configure_pwm_from_duty_pin_11(99);
		i = 1000000;
		while(i >= 0) i--;
	}
}

