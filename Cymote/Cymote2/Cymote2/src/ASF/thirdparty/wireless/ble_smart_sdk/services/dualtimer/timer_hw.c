#include <asf.h>
#include "timer_hw.h"
#include "conf_dualtimer.h"

extern struct uart_module uart_instance;


void hw_timer_init(void)
{
	struct dualtimer_config config_dualtimer;
	dualtimer_get_config_defaults(&config_dualtimer);

	config_dualtimer.timer1.load_value = CONF_DUALTIMER_TIMER1_LOAD;
	config_dualtimer.timer2.load_value = CONF_DUALTIMER_TIMER2_LOAD;

	dualtimer_init(&config_dualtimer);
}

void hw_timer_register_callback(hw_timer_callback_t timer_1_callback_handler, hw_timer_callback_t timer_2_callback_handler)
{
	dualtimer_register_callback(DUALTIMER_TIMER1, timer_1_callback_handler);
	dualtimer_register_callback(DUALTIMER_TIMER2, timer_2_callback_handler);

	dualtimer_disable(DUALTIMER_TIMER1);
	dualtimer_disable(DUALTIMER_TIMER2);

	NVIC_EnableIRQ(DUALTIMER0_IRQn);
}

void hw_timer_start(uint32_t timer_1_delay, uint32_t timer_2_delay)
{
	if(timer_1_delay <= 0) {
		timer_1_delay = 1;
	}

	if(timer_2_delay <= 0) {
		timer_2_delay = 1;
	}

	dualtimer_set_counter(DUALTIMER_TIMER1, DUALTIMER_SET_CURRUNT_REG, CONF_DUALTIMER_TIMER1_LOAD*timer_1_delay);
	dualtimer_enable(DUALTIMER_TIMER1);

	dualtimer_set_counter(DUALTIMER_TIMER2, DUALTIMER_SET_CURRUNT_REG, CONF_DUALTIMER_TIMER2_LOAD*timer_2_delay);
	dualtimer_enable(DUALTIMER_TIMER2);

}

void hw_timer_stop(void)
{
	dualtimer_disable(DUALTIMER_TIMER1);
	dualtimer_disable(DUALTIMER_TIMER2);
}
