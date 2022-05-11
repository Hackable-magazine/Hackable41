#include <stdint.h>
#include "uart.h"
#include "util.h"
#include "delay.h"
#include "mbox.h"
#include "fb.h"
#include "clock.h"
//#include "rick.h"
#include "HK39.h"

volatile unsigned int  __attribute__((aligned(16))) mbox[36];
//volatile unsigned int  __attribute__((section (".mbox"))) mbox[36];

int kernelmain(void)
{
	struct fb_t fb;

	// MINI UART REDUIT ET LOCK LA FREQ !!!!!! -> "The firmware locks the core frequency based on the enable_uart=1 "
	// "To enable it, add enable_uart=1 to config.txt. This will also fix the core frequency to 250MHz"
	// https://forums.raspberrypi.com/viewtopic.php?t=271783

	setmaxclock(CLK_ARM, mbox);
	setclock(CLK_UART, 3000000, mbox);

	init_uart(mbox);
	puts("\r\n\r\n\r\nBonjour Hackable !\r\n");

	if(!init_fb(mbox, &fb)) {
		puts("Framebuffer address: 0x");
		printhex((unsigned int)fb.fbp);
		puts("\r\n");
	} else {
		puts("Unable to get framebuffer!\r\n");
	}

    // get the board's unique serial number with a mailbox cal
	// https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
    mbox[0] = 8*4;                  // length of the message 8*4bytes
    mbox[1] = MBOX_REQUEST;         // this is a request message
    mbox[2] = MBOX_TAG_GETSERIAL;   // get serial number command
    mbox[3] = 8;                    // Response: Length: in bytes (expecting 2 bytes in [5][6])
    mbox[4] = 0;					// Request: Length: in bytes (bytes in [5][6] not matters)
    mbox[5] = 0;                    // clear output buffer
    mbox[6] = 0;
    mbox[7] = MBOX_TAG_LAST;

    // send the message to the GPU and receive answer
    if (mbox_call(MBOX_CH_PROP, mbox)) {
        puts("Serial number = ");
        printhex(mbox[6]);
        printhex(mbox[5]);
        puts("\r\n");
    } else {
        puts("Error getting serial number!\r\n");
    }

    mbox[0]=8*4;
    mbox[1]=MBOX_REQUEST;
    mbox[2]=MBOX_TAG_GETARMMEM;
    mbox[3]=8;
    mbox[4]=0;
    mbox[5]=0;
    mbox[6]=0;
    mbox[7]=MBOX_TAG_LAST;
    if (mbox_call(MBOX_CH_PROP, mbox)) {
        puts("ARM memory start at 0x");
        printhex(mbox[5]);
        puts(" - ");
		int mem=mbox[6]/1024/1024;
		printdec(mem);
		puts(" Mo\r\n");
    } else {
        puts("Error getting ARM memory!\r\n");
    }

    mbox[0]=8*4;
    mbox[1]=MBOX_REQUEST;
    mbox[2]=MBOX_TAG_GETVCOREMEM;
    mbox[3]=8;
    mbox[4]=0;
    mbox[5]=0;
    mbox[6]=0;
    mbox[7]=MBOX_TAG_LAST;
    if (mbox_call(MBOX_CH_PROP, mbox)) {
        puts("Videocore memory start at 0x");
        printhex(mbox[5]);
        puts(" - ");
		int mem=mbox[6]/1024/1024;
		printdec(mem);
		puts(" Mo\r\n");
    } else {
        puts("Error getting Videocore memory!\r\n");
    }

    mbox[0]=8*4; 					// length of the message en octets
    mbox[1]=MBOX_REQUEST;			// this is a request message
    mbox[2]=MBOX_TAG_GETCORETEMP;	// the tag id
    mbox[3]=8;						// buffer size (Response: Length: 8 in https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface) en octets
    mbox[4]=0;						// len (Request: Length: 4) en octets
    mbox[5]=0x000000001;			// ID
    mbox[6]=0;
    mbox[7]=MBOX_TAG_LAST;
    if (mbox_call(MBOX_CH_PROP, mbox)) {
        puts("SoC temperature: ");
		int temp=mbox[6];
		printdec(temp);
		puts("\r\n");
    } else {
        puts("Error getting SoC temperature!\r\n");
    }

	/*
    mbox[0]=8*4; 					// length of the message en octets
    mbox[1]=MBOX_REQUEST;			// this is a request message
    mbox[2]=MBOX_TAG_GETCLK;		// the tag id
    mbox[3]=8;						// buffer size (Response: Length: 8 in https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface) en octets
    mbox[4]=0;						// len (Request: Length: 4) en octets
    mbox[5]=0x000000003;			// ID ARM
    mbox[6]=0;
    mbox[7]=MBOX_TAG_LAST;
    if (mbox_call(MBOX_CH_PROP, mbox)) {
        puts("ARM current clock: ");
		printdec(mbox[6]);
		puts(" Hz\r\n");
    } else {
        puts("Error getting ARM clock!\r\n");
    }

    mbox[0]=8*4; 					// length of the message en octets
    mbox[1]=MBOX_REQUEST;			// this is a request message
    mbox[2]=MBOX_TAG_GETCLK;		// the tag id
    mbox[3]=8;						// buffer size (Response: Length: 8 in https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface) en octets
    mbox[4]=0;						// len (Request: Length: 4) en octets
    mbox[5]=0x000000002;			// ID uart
    mbox[6]=0;
    mbox[7]=MBOX_TAG_LAST;
    if (mbox_call(MBOX_CH_PROP, mbox)) {
        puts("UART current clock: ");
		printdec(mbox[6]);
		puts(" Hz\r\n");
    } else {
        puts("Error getting UART clock!\r\n");
    }
	*/
	printclock(mbox);

	puts("Exception level: ");
	printdec(get_el());
	puts("\r\n");

	if(!fb.fbp)
		return 0;
	puts("Framebuffer width: ");
	printdec(fb.width);
	puts("\r\n");
	puts("Framebuffer height: ");
	printdec(fb.height);
	puts("\r\n");
	puts("Framebuffer pitch: ");
	// the number of bytes per line = pitch
	printdec(fb.pitch);
	puts("\r\n");
	puts("Framebuffer pixel format: ");
	puts(fb.isrgb>0 ? "RGB\r\n" : "BGR\r\n");

	unsigned char *ptr=fb.fbp;
	uint64_t avant = micros();

	int x,y;
	char *imgdata=header_data;
	char pixel[4];

	// centrage
    ptr += (fb.height-height)/2*fb.pitch + (fb.width-width)*2;

	// RGB ou BGR ?
	if(fb.isrgb) {
		// RGB
		for(y=0;y<height;y++) {
			for(x=0;x<width;x++) {
				HEADER_PIXEL(imgdata, pixel);
				*((unsigned int*)ptr) = *((unsigned int *)&pixel);
				ptr+=4;
			}
			ptr+=fb.pitch-width*4;
		}
	} else {
		// BGR
		for(y=0;y<height;y++) {
			for(x=0;x<width;x++) {
				HEADER_PIXEL(imgdata, pixel);
				*((unsigned int*)ptr) = (unsigned int)(pixel[0]<<16|pixel[1]<<8|pixel[2]);
				ptr+=4;
			}
			ptr+=fb.pitch-width*4;
		}
	}

	puts("Time: ");
	printdec((micros()-avant));
	puts("µs\r\n");
	printdec(1000000/(micros()-avant));
	puts(" FPS\r\n");



	while(1) {
		;
	}

	return 0;
}
