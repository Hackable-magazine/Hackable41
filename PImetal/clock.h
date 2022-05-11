#pragma once

#define CLK_EMMC		0x00000001
#define CLK_UART		0x00000002
#define CLK_ARM			0x00000003
#define CLK_CORE		0x00000004
#define CLK_V3D			0x00000005
#define CLK_H264		0x00000006
#define CLK_ISP			0x00000007
#define CLK_SDRAM		0x00000008
#define CLK_PIXEL		0x00000009
#define CLK_PWM			0x0000000a
#define CLK_HEVC		0x0000000b
#define CLK_EMMC2		0x0000000c
#define CLK_M2MC		0x0000000d
#define CLK_PIXEL_BVB	0x0000000e

unsigned int getclock(uint32_t clkid, uint8_t max, volatile unsigned int *mbox);
void printclock(volatile unsigned int *mbox);
void setclock(uint32_t clkid, uint32_t freq, volatile unsigned int *mbox);
void setmaxclock(uint32_t clkid, volatile unsigned int *mbox);
