#include "TouchDriver.h"

/* Original code by Mario Becker, Fraunhofer IGD, 2007
    Thank you <3 for making it open source
*/
uint8_t readTouch(TouchPad *pad)
{
    volatile uint8_t *pin  = pad->pin;
    volatile uint8_t *port = pad->port;
    volatile uint8_t *ddr  = pad->ddr;

    uint8_t bitmask = pad->bitmask;

    /* Discharge pin*/
    *port &= ~(bitmask);
    *ddr  |= bitmask;
    Delay_MS(1);

    /* Save the status register */
    uint8_t MCUSR_old = MCUSR; // Not sure Why we need this?
    /* Turn off interrupts */
    GlobalInterruptDisable();
    /* Charge pin*/
    uint8_t cycles = 17;
    /* Make the pin an input with the internal pull-up on*/
    *ddr &= ~(bitmask);
    *port |= bitmask;
    /* Now see how long the pin to get pulled up. This manual unrolling of the loop
     decreases the number of hardware cycles between each read of the pin,
     thus increasing sensitivity. */
    if (*pin & bitmask) { cycles =  0;}
    else if (*pin & bitmask) { cycles =  1;}
    else if (*pin & bitmask) { cycles =  2;}
    else if (*pin & bitmask) { cycles =  3;}
    else if (*pin & bitmask) { cycles =  4;}
    else if (*pin & bitmask) { cycles =  5;}
    else if (*pin & bitmask) { cycles =  6;}
    else if (*pin & bitmask) { cycles =  7;}
    else if (*pin & bitmask) { cycles =  8;}
    else if (*pin & bitmask) { cycles =  9;}
    else if (*pin & bitmask) { cycles = 10;}
    else if (*pin & bitmask) { cycles = 11;}
    else if (*pin & bitmask) { cycles = 12;}
    else if (*pin & bitmask) { cycles = 13;}
    else if (*pin & bitmask) { cycles = 14;}
    else if (*pin & bitmask) { cycles = 15;}
    else if (*pin & bitmask) { cycles = 16;}

    /* Turn back on interrupts */
    GlobalInterruptEnable();
    MCUSR = MCUSR_old;

    /* Discharge pins again*/
    *port &= ~(bitmask);
    *ddr  |= bitmask;

    return(cycles);
}


/* Define touch pads as global struct here.
QUESTION: Is this the best way to do it? Funcitons and dynamic memory allocation?
Should I instead use a Macro function for this? This isn't very easy to change when I
come up wth a new board layout*/

/* On v1.0 of the board these are in the order of outer clockwise, then inner clockwise,
 starting fron the USB port.
*/

extern TouchPad touch1 =
	{
		.pin =  &PINF,
		.port = &PORTF,
		.ddr = &DDRF,
		.bitmask = 1 << 0,
        .state = 0,
	};

extern TouchPad touch2 =
	{
		.pin =  &PINF,
		.port = &PORTF,
		.ddr = &DDRF,
		.bitmask = 1 << 1,
        .state = 0,
	};

extern TouchPad touch3 =
	{
		.pin =  &PINF,
		.port = &PORTF,
		.ddr = &DDRF,
		.bitmask = 1 << 4,
        .state = 0,
	};

extern TouchPad touch4 =
	{
		.pin =  &PINF,
		.port = &PORTF,
		.ddr = &DDRF,
		.bitmask = 1 << 5,
        .state = 0,
	};

extern TouchPad touch5 =
	{
		.pin =  &PINF,
		.port = &PORTF,
		.ddr = &DDRF,
		.bitmask = 1 << 6,
        .state = 0,
	};

extern TouchPad touch6 =
	{
		.pin =  &PINF,
		.port = &PORTF,
		.ddr = &DDRF,
		.bitmask = 1 << 7,
        .state = 0,
	};

extern TouchPad touch7 =
	{
		.pin =  &PINC,
		.port = &PORTC,
		.ddr = &DDRC,
		.bitmask = 1 << 6,
        .state = 0,
	};

extern TouchPad touch8 =
	{
		.pin =  &PIND,
		.port = &PORTD,
		.ddr = &DDRD,
		.bitmask = 1 << 4,
        .state = 0,
	};

extern TouchPad touch9 =
	{
		.pin =  &PIND,
		.port = &PORTD,
		.ddr = &DDRD,
		.bitmask = 1 << 3,
        .state = 0,
	};

extern TouchPad touch10 =
	{
		.pin =  &PIND,
		.port = &PORTD,
		.ddr = &DDRD,
		.bitmask = 1 << 2,
        .state = 0,
	};

extern TouchPad touch11 =
	{
		.pin =  &PIND,
		.port = &PORTD,
		.ddr = &DDRD,
		.bitmask = 1 << 0,
        .state = 0,
	};

extern TouchPad touch12 =
	{
		.pin =  &PIND,
		.port = &PORTD,
		.ddr = &DDRD,
		.bitmask = 1 << 1,
        .state = 0,
	};

extern TouchPad touch13 =
	{
		.pin =  &PINB,
		.port = &PORTB,
		.ddr = &DDRB,
		.bitmask = 1 << 2,
        .state = 0,
	};

extern TouchPad touch14 =
	{
		.pin =  &PINB,
		.port = &PORTB,
		.ddr = &DDRB,
		.bitmask = 1 << 3,
        .state = 0,
    };

extern TouchPad touch15 =
	{
		.pin =  &PINB,
		.port = &PORTB,
		.ddr = &DDRB,
		.bitmask = 1 << 0,
        .state = 0,
	};

extern TouchPad touch16 =
	{
		.pin =  &PINE,
		.port = &PORTE,
		.ddr = &DDRE,
		.bitmask = 1 << 6,
        .state = 0,
	};

extern TouchPad touch17 =
	{
		.pin =  &PINB,
		.port = &PORTB,
		.ddr = &DDRB,
		.bitmask = 1 << 7,
        .state = 0,
	};

extern TouchPad touch18 =
	{
		.pin =  &PINC,
		.port = &PORTC,
		.ddr = &DDRC,
		.bitmask = 1 << 7,
        .state = 0,
	};

extern TouchPad touch19 =
	{
		.pin =  &PINB,
		.port = &PORTB,
		.ddr = &DDRB,
		.bitmask = 1 << 6,
        .state = 0,
	};

extern TouchPad touch20 =
	{
		.pin =  &PINB,
		.port = &PORTB,
		.ddr = &DDRB,
		.bitmask = 1 << 5,
        .state = 0,
	};

extern TouchPad touch21 =
	{
		.pin =  &PINB,
		.port = &PORTB,
		.ddr = &DDRB,
		.bitmask = 1 << 4,
        .state = 0,
	};

extern TouchPad touch22 =
	{
		.pin =  &PIND,
		.port = &PORTD,
		.ddr = &DDRD,
		.bitmask = 1 << 7,
        .state = 0,
	};

extern TouchPad touch23 =
	{
		.pin =  &PIND,
		.port = &PORTD,
		.ddr = &DDRD,
		.bitmask = 1 << 6,
        .state = 0,
	};

extern TouchPad touch24 =
	{
		.pin =  &PIND,
		.port = &PORTD,
		.ddr = &DDRD,
		.bitmask = 1 << 5,
        .state = 0,
	};

extern TouchPad* pads[24] = {&touch1, &touch2, &touch3, &touch4, &touch5, &touch6, &touch7, &touch8, &touch9, &touch10, &touch11, &touch12, &touch13, &touch14, &touch15, &touch16, &touch17, &touch18, &touch19, &touch20, &touch21, &touch22, &touch23, &touch24};