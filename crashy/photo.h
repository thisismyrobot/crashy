#ifndef photo_h
#define photo_h

#include <Arduino.h>

// esp-cam agnostic photo info
typedef struct {
    uint8_t * buf;
    size_t len;
} photo_fb_t;

#endif
