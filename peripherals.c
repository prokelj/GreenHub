#include "peripherals.h"
#include "timers.h"

void init_peripheral_pins(void) {

    //diode
    TRISFbits.TRISF6 = 0;
    LATFbits.LATF6 = 0;

    //periferije
    TRISDbits.TRISD8 = 1; //d8  ulaz za pir
    TRISDbits.TRISD9 = 0; //d9  izlaz za servo
    LATDbits.LATD9 = 0;

    //buzzer
    TRISAbits.TRISA11 = 0; //konfigurisemo A11 kao izlaz za pwm (buzzer)
    LATAbits.LATA11 = 0; //pocetna vrijednost
}

void init_pins(void) {
    init_peripheral_pins();
    init_adc_pins();
    init_ts_pins();
    init_glcd_pins();
}

void configure_modules(void) {
    configure_uart1();
    configure_adc();
    configure_glcd();
}

void update_lights(void) {
    //citamo proc 0-100%
    int proc = scale_light(fotootp);

    //clamp za sigurnost
    if (proc < 0) proc = 0;
    else if (proc > 100) proc = 100;

    if (proc >= 40) {
        LATFbits.LATF6 = 0;
    } else {
        LATFbits.LATF6 = 1;
    }

}

void open_doors(void) {
    if (serijska) {
        serial_print_string("Vrata su otvorena!\n\r");
    }
    buzzer_on = true;
    LATDbits.LATD9 = 1;
    delay_pola_ms(4); //2ms
    LATDbits.LATD9 = 0;
}

void close_doors(void) {
    if (serijska) {
        serial_print_string("Vrata su zatvorena!\n\r");
    }
    buzzer_on = true;
    LATDbits.LATD9 = 1;
    delay_pola_ms(3); //1.5ms
    LATDbits.LATD9 = 0;
}

void open_doors_manual(void) {
    if (serijska) {
        serial_print_string("Vrata su otvorena rucno!\n\r");
    }
    buzzer_on = true;
    LATDbits.LATD9 = 1;
    delay_pola_ms(2); //1ms
    LATDbits.LATD9 = 0;
}

//0-100%

unsigned int scale_light(unsigned int fo) {
    if (fo > 4095) fo = 4095;
    return ((unsigned long) fo * 100 + 2047) / 4095;
}

//0-100%

unsigned int scale_co2(unsigned int co2) {
    if (co2 > 4095) co2 = 4095;
    return ((unsigned long) (4095 - co2) * 100 + 2047) / 4095;
}

//citamo pir

bool pir_aktiviran(void) {
    bool pir = false;

    if (PORTDbits.RD8) //citamo pir senzor sa RD8 pina
        pir = true;
    else
        pir = false;

    return pir;
}

