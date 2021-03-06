/* 
 * nRF51822xxxx as TX and nRF24L01+ as Reciever
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

static uint8_t my_tx_payload[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14 }; // payload to send

// Initialise Enhanced Shockburst
static void esb_init(void) {
	
	// Set oscillator control to automatic
	nrf_esb_set_xosc_ctl(NRF_ESB_XOSC_CTL_AUTO);
	
	// Initialise TX mode
	nrf_esb_init(NRF_ESB_MODE_PTX);
	
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
	
	// Add packet into TX queue
	nrf_esb_add_packet_to_tx_fifo(PIPE_NUMBER, my_tx_payload, TX_PAYLOAD_LENGTH, NRF_ESB_PACKET_USE_ACK);

	nrf_esb_enable();

	while (1) {
		// Send a packet every 500ms
		nrf_delay_ms(500);
		nrf_esb_add_packet_to_tx_fifo(PIPE_NUMBER, my_tx_payload, TX_PAYLOAD_LENGTH, NRF_ESB_PACKET_USE_ACK);
	}

}
// If an ACK was received, we send another packet.
void nrf_esb_tx_success(uint32_t tx_pipe, int32_t rssi) {
	// Toggle LED0 to indicate success.
	nrf_gpio_pin_toggle(LED_0);
}

// If the transmission failed, send a new packet.
void nrf_esb_tx_failed(uint32_t tx_pipe) {
	// Toggle LED1 to indicate failure and flux TX fifo
	nrf_gpio_pin_toggle(LED_1);
	nrf_esb_flush_tx_fifo(PIPE_NUMBER);
}

// Not being used. As there no ACK Payload
void nrf_esb_rx_data_ready(uint32_t rx_pipe, int32_t rssi) {
	// Flushing Rx fifo so that there is space for future transmits
	nrf_esb_flush_rx_fifo(PIPE_NUMBER);
}

// Not being used
void nrf_esb_disabled(void) {
}

