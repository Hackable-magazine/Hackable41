#pragma once

#include "mmio.h"

// Auxiliary enables
#define AUX_ENB (*(volatile unsigned *)(MU_BASE + 0x04))
// Mini Uart I/O Data
#define MU_IO   (*(volatile unsigned *)(MU_BASE + 0x40))
// Mini Uart Line Control
#define MU_LCR  (*(volatile unsigned *)(MU_BASE + 0x4c))
// Mini Uart Line Status
#define MU_LSR  (*(volatile unsigned *)(MU_BASE + 0x54))
// Mini Uart Extra Control
#define MU_CNTL (*(volatile unsigned *)(MU_BASE + 0x60))
// Mini Uart Baudrate
#define MU_BAUD (*(volatile unsigned *)(MU_BASE + 0x68))

/* PL011 UART */
#define UART0_DR        (*(volatile unsigned *)(IO_BASE + 0x00201000))
#define UART0_FR        (*(volatile unsigned *)(IO_BASE + 0x00201018))
#define UART0_IBRD      (*(volatile unsigned *)(IO_BASE + 0x00201024))
#define UART0_FBRD      (*(volatile unsigned *)(IO_BASE + 0x00201028))
#define UART0_LCRH      (*(volatile unsigned *)(IO_BASE + 0x0020102C))
#define UART0_CR        (*(volatile unsigned *)(IO_BASE + 0x00201030))
#define UART0_IMSC      (*(volatile unsigned *)(IO_BASE + 0x00201038))
#define UART0_ICR       (*(volatile unsigned *)(IO_BASE + 0x00201044))

void init_uart();
void putc(char c);
