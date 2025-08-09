#ifndef STATE_MACHINE_H
#define	STATE_MACHINE_H

#include <stdbool.h>
#include <string.h> 
#include "glcd.h"
#include "serial.h"
#include "content.h"
#include "peripherals.h"

// === Touchscreen & State ===
extern bool touchActive; //debounce za touchscreen

// === Door State ===
extern bool mod_vrata; //true -> manual false -> auto
extern bool vrata_otvorena_zatvorena; //false -> zatvorena true -> otvorena
extern bool pir_reagovao; //da li je pir aktiviran

// === Display Buffers ===
extern bool co2_pixels_copied; //da li smo editovali display za co2
extern bool light_pixels_copied; //da li smo editovali display za svjetlo

// === Message prints ===
extern bool serijska; //debag preko serijske
extern bool manual_message_printed; //da li smo printali text u manual door modu
extern bool auto_message_printed; //da li smo printali text u auto door modu

typedef enum {
    STATE_HOME,
    STATE_MENU,
    STATE_DOOR_MANUAL,
    STATE_DOOR_AUTO_OPEN,
    STATE_DOOR_AUTO_CLOSED,
    STATE_CO2,
    STATE_LIGHT
} GLCD_STANJE;

extern GLCD_STANJE currentState;
extern GLCD_STANJE previousState;

bool touch_in_area(int x_min, int x_max, int y_min, int y_max);
void check_state(void);
void handle_state(void);
void update_display(GLCD_STANJE state);
void handle_home_display(void);
void handle_menu_display(void);
void handle_manual_door_display(void);
void handle_auto_door_display(void);
void handle_co2_display(void);
void handle_light_display(void);
void handle_reset_to_menu(void);

#endif	/* STATE_MACHINE_H */

