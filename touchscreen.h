#ifndef TOUCHSCREEN_H
#define	TOUCHSCREEN_H

#include<p30fxxxx.h>
#include "state_machine.h"

#define DRIVE_A PORTCbits.RC13
#define DRIVE_B PORTCbits.RC14

extern unsigned int X, Y, x_vrednost, y_vrednost;
extern const unsigned int AD_Xmin, AD_Xmax, AD_Ymin, AD_Ymax;

void delay(unsigned int N);
void init_ts_pins(void);
void read_x_y_coordinates(void);
void debounce_touchscreen(void);

#endif	/* TOUCHSCREEN_H */

