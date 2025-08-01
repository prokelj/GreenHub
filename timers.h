#ifndef TIMERS_H
#define	TIMERS_H

#include <p30f4013.h>
#include <stdbool.h>
#include "adc.h"
#include "serial.h"

extern volatile bool buzzer_on, glcd_update;
extern volatile unsigned int buzzer_br, brojac_pola_ms;

void configure_timers(void);

//TIMER1 ZA DELAY!
//-----------------------------------------------------------------------------------------------


#define TIMER1_PERIODA 1665 /*  Fosc = 3.33MHz
					          1/Fosc = 0.3us !!!, 0.3us *1665  = 0.5ms  */

void configure_timer1(void);
void delay_pola_ms(unsigned int br_pola_ms);
void delay_ms(unsigned int br_ms);
void delay_s(unsigned int br_s);
void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt(void);


//-----------------------------------------------------------------------------------------------


#define TIMER2_PERIODA 5203  //100ms sa prescalerom 64

void configure_timer2(void);
void __attribute__((__interrupt__,__auto_psv__)) _T2Interrupt(void);


//TIMER3 ZA PWM ZA BUZZER!
//-----------------------------------------------------------------------------------------------


#define TIMER3_PERIODA 900 //0.3us *900 = 0.27ms -> (1.8Khz perioda -> 3.6Khz pwm signal)

void configure_timer3(void);
void __attribute__((__interrupt__,__auto_psv__)) _T3Interrupt(void);

#endif	/* TIMERS_H */

