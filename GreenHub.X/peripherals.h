#ifndef PERIPHERALS_H
#define	PERIPHERALS_H

#include "timers.h"
#include "adc.h"
#include "state_machine.h"

void init_pins(void);
void init_peripheral_pins(void);
void configure_modules(void);
void update_lights(void);
void open_doors(void);
void open_doors_manual(void);
void close_doors(void);
unsigned int scale_light(unsigned int fo);
unsigned int scale_co2(unsigned int co2);
bool pir_aktiviran(void);

#endif	/* PERIPHERALS_H */

