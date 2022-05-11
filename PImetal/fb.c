#include <stdint.h>
#include "uart.h"
#include "util.h"
#include "mbox.h"
#include "fb.h"

#define SCREEN_WIDTH	1920 // 1366 // 1920
#define SCREEN_HEIGHT	1080 // 768 // 1080

int init_fb(volatile unsigned int *mbox, struct fb_t *fb)
{
	// Config FB
    mbox[0] = 35*4;
    mbox[1] = MBOX_REQUEST;
	// Résolution physique
    mbox[2] = MBOX_TAG_FBSETPHYS;
    mbox[3] = 8;
    mbox[4] = 0;
    mbox[5] = SCREEN_WIDTH; // y
    mbox[6] = SCREEN_HEIGHT; // x
	// Résolution virtuelle
	mbox[7] = MBOX_TAG_FBSETVIRT;
    mbox[8] = 8;
    mbox[9] = 0;
    mbox[10] = SCREEN_WIDTH;
    mbox[11] = SCREEN_HEIGHT;
	// Décalage résolutions
	mbox[12] = MBOX_TAG_FBSETOFF;
    mbox[13] = 8;
    mbox[14] = 0;
    mbox[15] = 0;
    mbox[16] = 0;
	// Nombre de couleurs
	mbox[17] = MBOX_TAG_FBSETDEPTH;
    mbox[18] = 4;
    mbox[19] = 0;
    mbox[20] = 32; // 8*4
	// Format pixels
	mbox[21] = MBOX_TAG_FBSETPIXORD;
    mbox[22] = 4;
    mbox[23] = 0;
    mbox[24] = 1; //RGB plutôt que BGR
	// Allocation framebuffer
	mbox[25] = MBOX_TAG_FBALLOC;
    mbox[26] = 8;
    mbox[27] = 0;
    mbox[28] = 4096; // alignement en octets
    mbox[29] = 0;
	// Octets par ligne
	mbox[30] = MBOX_TAG_FBGETPITCH;
    mbox[31] = 4;
    mbox[32] = 0;
    mbox[33] = 0;
	// Fin
	mbox[34] = MBOX_TAG_LAST;

	if (mbox_call(MBOX_CH_PROP, mbox) && mbox[20]==32 && mbox[28]!=0) {
        mbox[28]&=0x3FFFFFFF;   // bus address vers physical address
        fb->width=mbox[5];      // largeur
        fb->height=mbox[6];     // hauteur
        fb->pitch=mbox[33];     // pitch
        fb->isrgb=mbox[24];     // RGB ou BGR
        fb->fbp=(void*)((unsigned long)mbox[28]); // pointeur
    } else {
        puts("Unable to set framebuffer!\r\n");
		return 1;
    }

	return 0;
}
