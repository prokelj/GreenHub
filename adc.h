#include<p30fxxxx.h>
#include "touchscreen.h"

extern unsigned int x, y, fotootp, mq3;

void __attribute__((__interrupt__, __auto_psv__)) _ADCInterrupt(void);
void init_adc_pins(void);
void configure_adc(void);
