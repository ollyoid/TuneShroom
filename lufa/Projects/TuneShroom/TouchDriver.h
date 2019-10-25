#ifndef __TOUCH_DRIVER_H__
#define __TOUCH_DRIVER_H__

#if defined(__cplusplus)
    extern "C" {
#endif
#include "TuneShroom.h" 

typedef struct TouchPad{
    volatile uint8_t* pin;
    volatile uint8_t* port;
    volatile uint8_t* ddr;
    uint8_t bitmask;
    uint8_t state;
} TouchPad;

uint8_t readTouch(TouchPad *pad);

TouchPad touch1;
TouchPad touch2;
TouchPad touch3;
TouchPad touch4;
TouchPad touch5;
TouchPad touch6;
TouchPad touch7;
TouchPad touch8;
TouchPad touch9;
TouchPad touch10;
TouchPad touch11;
TouchPad touch12;
TouchPad touch13;
TouchPad touch14;
TouchPad touch15;
TouchPad touch16;
TouchPad touch17;
TouchPad touch18;
TouchPad touch19;
TouchPad touch20;
TouchPad touch21;
TouchPad touch22;
TouchPad touch23;
TouchPad touch24;

TouchPad* pads[24];



/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif


#endif