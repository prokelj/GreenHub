#include "touchscreen.h"

unsigned int X = 0, Y = 0;
unsigned int x_vrednost = 0, y_vrednost = 0;

const unsigned int AD_Xmin = 220;
const unsigned int AD_Xmax = 3642;
const unsigned int AD_Ymin = 520;
const unsigned int AD_Ymax = 3450;

void init_ts_pins(void)
{
    TRISCbits.TRISC13 = 0;
    TRISCbits.TRISC14 = 0;
}

void delay(unsigned int N)
{
    unsigned int i;
    for (i = 0; i < N; i++);
}

void read_x_y_coordinates(void)
{
    // vode horizontalni tranzistori
    DRIVE_A = 1;  
	DRIVE_B = 0;

    delay(500); // cekamo jedno vreme da se odradi AD konverzija
    x_vrednost = x;

    // vode vertikalni tranzistori
    DRIVE_A = 0;  
    DRIVE_B = 1;

    delay(500); // cekamo jedno vreme da se odradi AD konverzija
    y_vrednost = y;

    // Skaliranje
    X = (x_vrednost - 161) * 0.03629;
    Y = (y_vrednost - 500) * 0.020725;
}

void debounce_touchscreen(void){
    if (X < 0 || X > 128 || Y < 0 || Y > 64) {
            touchActive = true;
        }
}

