#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "simple_uart.h"
#include "nrf_gpio.h"


// Defines
#define ERROR_PIN			18
#define RX_PIN_NUMBER	11
#define TX_PIN_NUMBER	9

// quit uart operation
static void uart_quit()
{
  simple_uart_putstring((const uint8_t *)" \n\rExit!\n\r");
}

// start uart oeration
static void uart_start()
{
  simple_uart_putstring((const uint8_t *)" \n\rStart: ");
}

int main(void)
{
  simple_uart_config(0, TX_PIN_NUMBER, 0, RX_PIN_NUMBER, false);

  uart_start();
  while(true)
  {
    uint8_t cr = simple_uart_get();
    simple_uart_put(cr);

    if(cr == 'q' || cr == 'Q')
    {
      uart_quit();
      while(1){}
    }
  }
}
