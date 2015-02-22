/* 
 * Template file for working with nRF51822xxAA
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"

int main(void)
{
  nrf_gpio_cfg_output(18);
	nrf_gpio_cfg_output(19);
	
  while(true)
  {
    nrf_gpio_pin_clear(18);
		nrf_gpio_pin_set(19);
		
		nrf_delay_ms(500);
		
		nrf_gpio_pin_clear(19);
		nrf_gpio_pin_set(18);
		
		nrf_delay_ms(500);
  }
}

