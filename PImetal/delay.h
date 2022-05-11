#pragma once

#define STC_LOW (*(volatile unsigned *)(IO_BASE + 0x3004))
#define STC_HIGH (*(volatile unsigned *)(IO_BASE + 0x3008))

void wait_usec_system_timer(uint32_t number_of_micros_to_wait);
uint64_t micros();
