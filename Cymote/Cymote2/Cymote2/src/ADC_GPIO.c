#include "ADC_GPIO.h"

void configure_adc_4(void)
{
	struct adc_config config_adc;

	// Vertical

	adc_get_config_defaults(&config_adc);

	//ADC_INPUT_CH_GPIO_MS2 magic number
	config_adc.input_channel = 1;

	adc_init(&config_adc);

	adc_enable();
}

void configure_adc_3(void)
{
	struct adc_config config_adc;

	// Horizontal

	adc_get_config_defaults(&config_adc);

	//ADC_INPUT_CH_GPIO_MS1 magic number
	config_adc.input_channel = 0;

	adc_init(&config_adc);

	adc_enable();
}

void configure_gpio_pins(void)
{
	struct gpio_config config_gpio_pin;

	gpio_get_config_defaults(&config_gpio_pin);

	// Button assignments from left to right
	config_gpio_pin.direction = GPIO_PIN_DIR_INPUT;
	config_gpio_pin.input_pull = GPIO_PIN_PULL_UP;
	gpio_pin_set_config(BUTTON1, &config_gpio_pin);
	gpio_pinmux_cofiguration(BUTTON1, GPIO_PINMUX_SEL_0);

	config_gpio_pin.direction = GPIO_PIN_DIR_INPUT;
	config_gpio_pin.input_pull = GPIO_PIN_PULL_UP;
	gpio_pin_set_config(BUTTON2, &config_gpio_pin);
	gpio_pinmux_cofiguration(BUTTON2, GPIO_PINMUX_SEL_0);

	config_gpio_pin.direction = GPIO_PIN_DIR_INPUT;
	config_gpio_pin.input_pull = GPIO_PIN_PULL_UP;
	gpio_pin_set_config(BUTTON3, &config_gpio_pin);
	gpio_pinmux_cofiguration(BUTTON3, GPIO_PINMUX_SEL_0);

	config_gpio_pin.direction = GPIO_PIN_DIR_INPUT;
	config_gpio_pin.input_pull = GPIO_PIN_PULL_UP;
	gpio_pin_set_config(BUTTON4, &config_gpio_pin);
	gpio_pinmux_cofiguration(BUTTON4, GPIO_PINMUX_SEL_0);

	// LEDs

	// Red
	config_gpio_pin.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(RED_GPIO, &config_gpio_pin);
	gpio_pinmux_cofiguration(RED_GPIO, GPIO_PINMUX_SEL_0);

	// Green
	config_gpio_pin.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(GREEN_GPIO, &config_gpio_pin);
	gpio_pinmux_cofiguration(GREEN_GPIO, GPIO_PINMUX_SEL_0);

	// Blue
	config_gpio_pin.direction = GPIO_PIN_DIR_OUTPUT;
	gpio_pin_set_config(BLUE_GPIO, &config_gpio_pin);
	gpio_pinmux_cofiguration(BLUE_GPIO, GPIO_PINMUX_SEL_0);

}