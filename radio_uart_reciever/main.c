/* 
 * RF Reciever. Recieves a byte over RF and writes to UART
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "simple_uart.h"
#include "radio_config.h"

// Defines for LEDs
#define LED_0 18
#define LED_1 19

// Defines for UART
#define TX_PIN_NUMBER 9
#define RX_PIN_NUMBER 11

// payload for Radio TX/RX
uint8_t packet[1];

// quit uart operation
static void uart_quit()
{
	simple_uart_putstring((const uint8_t *)"Exit!\n\r");
}

// start uart operation
static void uart_start()
{
	simple_uart_putstring((const uint8_t *)"\n\rReciever\n\r");
}

void init()
{
	// Start 16 MHz crystal oscillator
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
	
	// Wait for oscillator to start up
	while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
	{
	}
	
	// Set radio configuration parameters
	radio_configure();
	
}

int main(void)
{
  //Initialise radio
	init();
	
	// Configure UART
	simple_uart_config(0, TX_PIN_NUMBER, 0, RX_PIN_NUMBER, false);
	
	// Configure LEDs as output
	nrf_gpio_cfg_output(18);
	nrf_gpio_cfg_output(19);
	nrf_gpio_pin_clear(LED_0);
	
	// Start uart operation
	uart_start();
	
  while(true)
  {
    // Set payload pointer
    NRF_RADIO->PACKETPTR = (uint32_t)packet;
    NRF_RADIO->EVENTS_READY = 0;
		
    // Enable radio and wait for ready
    NRF_RADIO->TASKS_RXEN = 1;
    while(NRF_RADIO->EVENTS_READY == 0U)
    {
    }
    NRF_RADIO->EVENTS_END = 0;
    // Start listening and wait for address received event
    NRF_RADIO->TASKS_START = 1;
    // Wait for end of packet
    while(NRF_RADIO->EVENTS_END == 0)
    {
    }
		nrf_gpio_pin_toggle(LED_0);
    // Write received data to LED0 and LED1 on CRC match
    if (NRF_RADIO->CRCSTATUS == 1)
    {
      // Write output to UART
			simple_uart_put(packet[0]);
    }
    NRF_RADIO->EVENTS_DISABLED = 0;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1;
    while(NRF_RADIO->EVENTS_DISABLED == 0)
    {
    }
  }
}

