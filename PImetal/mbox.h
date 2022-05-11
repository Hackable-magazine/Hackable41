#pragma once

#include "mmio.h"

#define MBOX0_READ       (*(volatile unsigned *)(VCORE_MBOX+0x0))
#define MBOX0_PEAK       (*(volatile unsigned *)(VCORE_MBOX+0x10))
#define MBOX0_SENDER     (*(volatile unsigned *)(VCORE_MBOX+0x14))
#define MBOX0_STATUS     (*(volatile unsigned *)(VCORE_MBOX+0x18))
#define MBOX0_CONFIG     (*(volatile unsigned *)(VCORE_MBOX+0x1C))
#define MBOX0_WRITE      (*(volatile unsigned *)(VCORE_MBOX+0x20))
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000
#define MBOX_REQUEST    0

/* channels */
#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

/* tags */
#define MBOX_TAG_GETSERIAL		0x00010004
#define MBOX_TAG_GETARMMEM		0x00010005
#define MBOX_TAG_GETVCOREMEM	0x00010006
#define MBOX_TAG_GETCORETEMP	0x00030006
#define MBOX_TAG_GETMAXCLK		0x00030004
#define MBOX_TAG_GETCLK			0x00030047
#define MBOX_TAG_SETCLK			0x00038002

#define MBOX_TAG_FBSETPHYS		0x48003
#define MBOX_TAG_FBSETVIRT		0x48004
#define MBOX_TAG_FBSETOFF		0x48009
#define MBOX_TAG_FBSETDEPTH		0x48005
#define MBOX_TAG_FBSETPIXORD	0x48006
#define MBOX_TAG_FBALLOC		0x40001
#define MBOX_TAG_FBGETPITCH		0x40008

#define MBOX_TAG_LAST			0

int mbox_call(unsigned char ch, volatile unsigned int *mbox);
