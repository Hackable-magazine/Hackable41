#pragma once

#include "mmio.h"

// GPIO Function Select 1
#define GPFSEL1 (*(volatile unsigned *)(GP_BASE + 0x04))
// GPIO Pin Pull-up/down Enable
#define GPPUD   (*(volatile unsigned *)(GP_BASE + 0x94))
// GPIO Pin Pull-up/down Enable Clock 0
#define GPPUDCLK0   (*(volatile unsigned *)(GP_BASE + 0x98))
