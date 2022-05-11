#include <stdint.h>
#include "uart.h"
#include "mbox.h"
#include "util.h"
#include "clock.h"

unsigned int getclock(uint32_t clkid, uint8_t max, volatile unsigned int *mbox)
{
	mbox[0]=8*4;
	mbox[1]=MBOX_REQUEST;
	mbox[2]= max>0 ? MBOX_TAG_GETMAXCLK : MBOX_TAG_GETCLK;
	mbox[3]=8;
	mbox[4]=0;
	mbox[5]=clkid;
	mbox[6]=0;
	mbox[7]=MBOX_TAG_LAST;
	if (mbox_call(MBOX_CH_PROP, mbox)) {
		return(mbox[6]);
	} else {
		puts("Error getting clock!\r\n");
		return(0);
	}
}

void setclock(uint32_t clkid, uint32_t freq, volatile unsigned int *mbox)
{
	mbox[0]=9*4;
	mbox[1]=MBOX_REQUEST;
	mbox[2]=MBOX_TAG_SETCLK;
	mbox[3]=12;
	mbox[4]=0;
	mbox[5]=clkid;
	mbox[6]=freq;
	mbox[7]=0;
	mbox[8]=MBOX_TAG_LAST;
	// WARNING : silent error
	mbox_call(MBOX_CH_PROP, mbox);
}

void printclock(volatile unsigned int *mbox)
{
	static const char *clocktxts[] = {
		"reserved", "EMMC", "UART", "ARM", "CORE",
		"V3D", "H264", "ISP", "SDRAM", "PIXEL", "PWM",
		"HEVC", "EMMC2", "M2MC", "PIXEL_BVB"
	};

	puts("Clocks:\r\n");
	for(int i=1; i<0xf; i++) {
		puts("  ");
		puts(clocktxts[i]);
		puts(": ");
		printdec(getclock(i, 0, mbox));
		puts(" / ");
		printdec(getclock(i, 1, mbox));
		puts(" Hz\r\n");
	}
}

void setmaxclock(uint32_t clkid, volatile unsigned int *mbox)
{
	uint32_t maxfreq = getclock(clkid, 1, mbox);
	setclock(clkid, maxfreq, mbox);
}
