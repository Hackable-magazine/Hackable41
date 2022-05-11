#pragma once

struct fb_t {
    unsigned char *fbp;
    unsigned int width;
    unsigned int height;
    unsigned int pitch;
    unsigned int isrgb;
};

int init_fb(volatile unsigned int *mbox, struct fb_t *fb);
