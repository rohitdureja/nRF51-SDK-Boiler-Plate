/* 
 * Template file for working with nRF51822xxAA
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf.h"
#include "nrf_delay.h"

#define PIN_OUT 18

static void gpio_init(void)
{
	nrf_gpio_cfg_output(PIN_OUT);
	nrf_gpio_pin_clear(PIN_OUT);
}	

static void timer0_init(void)
{
	// Start the 16MHz crystal oscillator
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
	
	// Wait for external oscillator to start
	while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
	{
		
	}
	
	// Clear Timer0
	NRF_TIMER0->TASKS_CLEAR = 1;
	
	// Enable interrupts
	NVIC_EnableIRQ(TIMER0_IRQn);

	// Configure Timer0 for compare[0] event every 500ms
	NRF_TIMER0->PRESCALER = 4;
	NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;
	NRF_TIMER0->BITMODE = (TIMER_BITMODE_BITMODE_24Bit << TIMER_BITMODE_BITMODE_Pos);
	NRF_TIMER0->CC[0] = 500 * 1000UL;
	
	// Enable interrupt
	NRF_TIMER0->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;
	NRF_TIMER0->SHORTS    =
        (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);
	
	// Start timer
	NRF_TIMER0->TASKS_START = 1;
}

void TIMER0_IRQHandler(void)
{
	if (NRF_TIMER0->EVENTS_COMPARE[0] != 0)
  {
    nrf_gpio_pin_toggle(PIN_OUT);
		NRF_TIMER0->EVENTS_COMPARE[0] = 0;
  }
}

int main(void)
{
  gpio_init();
	timer0_init();
	
  while(true)
  {
    
  }
}

