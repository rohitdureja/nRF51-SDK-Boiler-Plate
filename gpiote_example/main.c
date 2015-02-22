/* 
 * Template file for working with nRF51822xxAA
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf.h"
#include "nrf_delay.h"

#define PIN_OUT 18

static void gpiote_init(void) 
{
	// Configure gpio port as output
	nrf_gpio_cfg_output(PIN_OUT);
	
	// Configure GPIOTE channel 0 to toggle GPIO state with input pin.
	nrf_gpiote_task_config(0, PIN_OUT, NRF_GPIOTE_POLARITY_TOGGLE, \
													NRF_GPIOTE_INITIAL_VALUE_LOW);
}

static void ppi_init(void)
{
	// Configure PPI channel 0 to toggle GPIO_OUTPUT_PIN on every TIMER0 COMPARE[0] match (200 ms)
	NRF_PPI->CH[0].EEP = (uint32_t)&NRF_TIMER0->EVENTS_COMPARE[0];
	NRF_PPI->CH[0].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[0];

	// Enable PPI channel 0
	NRF_PPI->CHEN = (PPI_CHEN_CH0_Enabled << PPI_CHEN_CH0_Pos);
}

static void timer0_init(void)
{
	// Start 16 MHz crystal oscillator.
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART    = 1;

	// Wait for the external oscillator to start.
	while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
	{
		
	}

	// Clear TIMER0
	NRF_TIMER0->TASKS_CLEAR = 1;

	// Configure TIMER0 for compare[0] event every 200 ms.
	NRF_TIMER0->PRESCALER = 4;            // Prescaler 4 results in 1 tick equals 1 microsecond.
	NRF_TIMER0->CC[0]     = 200 * 1000UL; // 1 tick equals 1µ , multiply by 1000 for ms value.
	NRF_TIMER0->MODE      = TIMER_MODE_MODE_Timer;
	NRF_TIMER0->BITMODE   = TIMER_BITMODE_BITMODE_24Bit;
	NRF_TIMER0->SHORTS    = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);
}

int main(void)
{
  gpiote_init(); // Configure a GPIO to toggle on a GPIOTE task.
	timer0_init(); // Use TIMER0 to generate events every 200 ms.
	ppi_init();    // Use a PPI channel to connect the event to the task automatically.

	NRF_TIMER0->TASKS_START = 1;  // Start event generation.

	while (true)
	{
			
	}
}

