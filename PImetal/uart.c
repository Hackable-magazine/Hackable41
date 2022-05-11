#include <stdint.h>
#include "uart.h"
#include "gpio.h"
#include "mbox.h"
#include "clock.h"

// https://github.com/mrvn/RaspberryPi-baremetal/blob/master/008-trust-but-verify/uart.c

void init_uart()
{
	// wait for device
	while(UART0_FR & (1 << 3)) { ; }

	UART0_CR = 0;         // turn off UART0

	// GPIO 14 & 15 en alt0
	GPFSEL1 &= ~((7 << 12) | (7 << 15));
	GPFSEL1 |=   (4 << 12) | (4 << 15);
	GPPUD = 0;            // enable pins 14 and 15
	for(uint8_t i = 0; i < 150; i++)
		asm volatile ("nop");
	GPPUDCLK0 = (1 << 14)|(1 << 15);
	for(uint8_t i = 0; i < 150; i++)
		asm volatile ("nop");
	GPPUDCLK0 = 0;        // flush GPIO setup

	UART0_ICR = 0x7FF;    // clear interrupts

	// Set integer & fractional part of baud rate.
	// Divider = UART_CLOCK/(16 * Baud)
	// Fraction part register = (Fractional part * 64) + 0.5
	// UART_CLOCK = 3000000; Baud = 115200.
	// Divider = 3000000/(16 * 115200) = 1.627 = ~1.
	// Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
	UART0_IBRD = 1;       // 115200 baud
	UART0_FBRD = 40;
	UART0_LCRH = 0b11<<5; // 8n1
	UART0_CR = 0x301;     // enable Tx, Rx, FIFO
}

void putc(char c)
{
	while(UART0_FR & (1 << 5)) { ; }
	UART0_DR = c;
}
