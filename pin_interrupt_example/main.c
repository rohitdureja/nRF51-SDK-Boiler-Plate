/* 
 * Template file for working with nRF51822xxAA
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_gpiote.h"

#define PIN_OUT 18
#define PIN_IN 16

static void gpio_init(void)
{
	nrf_gpio_cfg_output(PIN_OUT);
	nrf_gpio_cfg_input(PIN_IN, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_pin_clear(PIN_OUT);
	
	//Enable Interrupt
	NVIC_EnableIRQ(GPIOTE_IRQn);
	/*NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)
                            | (PIN_IN << GPIOTE_CONFIG_PSEL_Pos)
                            | (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos)
														| (GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos);*/
	nrf_gpiote_event_config(0, PIN_IN, GPIOTE_CONFIG_POLARITY_Toggle);
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;
}

void GPIOTE_IRQHandler(void)
{
	// Event causing the interrupt must be cleared.
	if ((NRF_GPIOTE->EVENTS_IN[0] == 1) &&
		(NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN0_Msk))
	{
		NRF_GPIOTE->EVENTS_IN[0] = 0;
		nrf_gpio_pin_toggle(PIN_OUT);
	}
}
int main(void)
{
 
	gpio_init();
	
  while(true)
  {
		
  }
}

