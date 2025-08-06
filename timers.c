#include "timers.h"

volatile bool buzzer_on = false;
volatile bool glcd_update = false;
volatile unsigned int buzzer_br = 0;
volatile unsigned int brojac_pola_ms = 0;
volatile unsigned int brojac_glcd_update = 0;

void configure_timers(void){
    configure_timer1();
    configure_timer2();
    configure_timer3();
}

void configure_timer1(void) {

    T1CONbits.TON = 0;
    TMR1 = 0; // Clear the timer register
    PR1 = TIMER1_PERIODA; //0.5ms
    T1CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
    IPC0bits.T1IP = 2; //prioritet tajmera 1 najvisi jer se najcesce desava
    IFS0bits.T1IF = 0; // clear interrupt flag
    IEC0bits.T1IE = 1; // enable interrupt
    T1CONbits.TON = 1; // T1 on 
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    TMR1 = 0; // Reset the timer
    brojac_pola_ms++; // Increment 0.5 ms counter
    IFS0bits.T1IF = 0; // Clear the interrupt flag
}

void delay_pola_ms(unsigned int br_pola_ms) {
    brojac_pola_ms = 0;
    while (brojac_pola_ms < br_pola_ms);
}

void delay_ms(unsigned int br_ms) {
    brojac_pola_ms = 0;
    while (brojac_pola_ms < br_ms * 2);
}

void delay_s(unsigned int br_s) {
    delay_ms(1000 * br_s);
}

void configure_timer2(void) {

    T2CONbits.TON = 0;
    TMR2 = 0; // Clear the timer register
    PR2 = TIMER2_PERIODA;
    T2CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
    IPC1bits.T2IP = 2; //prioritet
    T2CONbits.TCKPS = 0b10; //prescaler 64
    IFS0bits.T2IF = 0; // clear interrupt flag
    IEC0bits.T2IE = 1; // enable interrupt
    T2CONbits.TON = 1; // Start Timer2
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
    TMR2 = 0;
    read_x_y_coordinates();
    debounce_touchscreen();
    ++brojac_glcd_update;
    if (brojac_glcd_update >= 5) {
        glcd_update = true;
        brojac_glcd_update = 0;
    }
    IFS0bits.T2IF = 0;
}

void configure_timer3(void) {

    T3CONbits.TON = 0;
    TMR3 = 0; // Clear the timer register
    PR3 = TIMER3_PERIODA;
    T3CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
    IPC1bits.T3IP = 1; //prioritet tajmera 3
    IFS0bits.T3IF = 0; // clear interrupt flag
    IEC0bits.T3IE = 1; // enable interrupt
    T3CONbits.TON = 1; // T3 on 
}

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void) {
    TMR3 = 0; //reset tajmera

    //logika za buzzer
    if (buzzer_on) {
        ++buzzer_br;
        if (buzzer_br >= 2500) {
            LATAbits.LATA11 = 0;
            buzzer_on = false;
            buzzer_br = 0;
        } else {
            LATAbits.LATA11 = !LATAbits.LATA11;
        }
    }

    IFS0bits.T3IF = 0; //clearujemo flag

}
