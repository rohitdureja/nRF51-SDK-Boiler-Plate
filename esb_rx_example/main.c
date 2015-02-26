/* 
 * nRF51822xxxx as RX and nRF24L01+ as Transmitter
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "esb/nrf_esb.h"
#include "simple_uart.h"

// Defines for LEDs
#define LED_0 18
#define LED_1 19

// Defines for UART pins
#define RX_PIN_NUMBER	11
#define TX_PIN_NUMBER	9

// Defines for ESB
#define PIPE_NUMBER 0 // use pipe 0
#define TX_PAYLOAD_LENGTH 20 // payload length of 10 bytes

static uint8_t my_rx_payload[20]; // payload received

// Initialise Enhanced Shockburst
static void esb_init(void) {
	
	// Set oscillator control to automatic
	nrf_esb_set_xosc_ctl(NRF_ESB_XOSC_CTL_AUTO);
	
	// Initialise RX mode
	nrf_esb_init(NRF_ESB_MODE_PRX);
	
	// Set length of CRC to 2 bytes
	nrf_esb_set_crc_length(NRF_ESB_CRC_LENGTH_2_BYTE);
	
	// Set datarate to 2mbps
	nrf_esb_set_datarate(NRF_ESB_DATARATE_2_MBPS);
	
	// Set retransmit delay to 600us
	nrf_esb_set_retransmit_delay(600);
	
	// Set number of retries
	nrf_esb_set_max_number_of_tx_attempts(5);
	
	// Enable pipe 0
	nrf_esb_set_enabled_prx_pipes(0x03);
	
	// Set base address length
	nrf_esb_set_base_address_length(NRF_ESB_BASE_ADDRESS_LENGTH_2B);
	
	// Set prefix byte of address for Pipe 0 (MSB)
	nrf_esb_set_address_prefix_byte(0, 0x2C);
	
	// Set base address for pipe 0
	nrf_esb_set_base_address_0(0x3E3E);
	
	// Set communication channel to 20 (2400Mhz + 20)
	nrf_esb_set_channel(20);

}

int main(void) {

	// Initialise status LEDs
	nrf_gpio_cfg_output(LED_0);
	nrf_gpio_cfg_output(LED_1);
	nrf_gpio_pin_clear(LED_0);
	nrf_gpio_pin_clear(LED_1);
	
	// Initialise UART
	simple_uart_config(0, TX_PIN_NUMBER, 0, RX_PIN_NUMBER, false);
	
	// Initialise ESB
	esb_init();

	nrf_esb_enable();

	while (1) {
	}

}
// Not being used. If an ACK was received, we send another packet.
void nrf_esb_tx_success(uint32_t tx_pipe, int32_t rssi) {
	
}

// Not being used. If the transmission failed, send a new packet.
void nrf_esb_tx_failed(uint32_t tx_pipe) {
	
}

// Called when data recieved over RADIO RX
void nrf_esb_rx_data_ready(uint32_t rx_pipe, int32_t rssi) {
	uint32_t rx_payload_length;
	
	// Fetch data from RX FIFO
	nrf_esb_fetch_packet_from_rx_fifo(PIPE_NUMBER, my_rx_payload, &rx_payload_length);
	if(rx_payload_length > 0) {
		nrf_gpio_pin_toggle(LED_0);
		simple_uart_putstring(my_rx_payload);
	}
	// Flushing Rx fifo so that there is space for future transmits
	nrf_esb_flush_rx_fifo(PIPE_NUMBER);
}

// Not being used
void nrf_esb_disabled(void) {
}

