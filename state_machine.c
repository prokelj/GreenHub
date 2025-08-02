#include "state_machine.h"

// === Touchscreen & State ===
bool touchActive = true; //debounce
GLCD_STANJE currentState = STATE_HOME;
GLCD_STANJE previousState = -1;

// === Door State ===
bool mod_vrata = false; //true -> manual false -> auto
bool vrata_otvorena_zatvorena = false; //false -> zatvorena true -> otvorena
bool pir_reagovao = false; //da li je pir aktiviran

// === Display Buffers ===
bool co2_pixels_copied = false; //da li smo editovali display za co2
bool light_pixels_copied = false; //da li smo editovali display za svjetlo

// === Message prints ===
bool manual_message_printed = false; //da li smo printali text u manual door modu
bool auto_message_printed = false; //da li smo printali text u auto door modu


//fja za ts

bool touch_in_area(int x_min, int x_max, int y_min, int y_max) {
    return (touchActive && X > x_min && X < x_max && Y > y_min && Y < y_max);
}

void check_state(void) {
    if (currentState != previousState) {
        update_display(currentState);
        previousState = currentState;
    }
}

void handle_state(void) {

    switch (currentState) {
        case STATE_HOME:
            handle_home_display();
            break;
        case STATE_MENU:
            handle_menu_display();
            break;
        case STATE_DOOR_MANUAL:
            handle_manual_door_display();
            break;
        case STATE_DOOR_AUTO_OPEN:
        case STATE_DOOR_AUTO_CLOSED:
            handle_auto_door_display();
            break;
        case STATE_CO2:
            handle_co2_display();
            break;
        case STATE_LIGHT:
            handle_light_display();
            break;
        default:
            handle_home_display();
            break;
    }
}

void update_display(GLCD_STANJE state) {
    switch (state) {
        case STATE_HOME:
            GLCD_DisplayPictureConst(home_display);
            if (serijska) {
                serial_print_string("Dobrodosli na pocetnu stranicu!!\n\r");
            }
            break;
        case STATE_MENU:
            GLCD_DisplayPictureConst(menu_display);
            if (serijska) {
                serial_print_string("Dobrodosli u meni!\n\r");
            }
            break;
        case STATE_DOOR_MANUAL:
            GLCD_DisplayPictureConst(vrata_disp_manual);
            if (serijska) {
                serial_print_string("Manuelni prikaz vrata!\n\r");
            }
            break;
        case STATE_DOOR_AUTO_OPEN:
            GLCD_DisplayPictureConst(vrata_disp_auto_open);
            break;
        case STATE_DOOR_AUTO_CLOSED:
            GLCD_DisplayPictureConst(vrata_disp_auto_closed);
            break;
        case STATE_CO2:
            GLCD_DisplayPicture(perc_bar);
            if (serijska) {
                serial_print_string("Prikaz CO2!\n\r");
            }
            break;
        case STATE_LIGHT:
            GLCD_DisplayPicture(perc_bar);
            if (serijska) {
                serial_print_string("Prikaz svetla!\n\r");
            }
            break;
        default:
            GLCD_DisplayPictureConst(home_display);
            if (serijska) {
                serial_print_string("Dobrodosli na pocetnu stranicu!!\n\r");
            }
    }
}


//fje za odredjene ekrane

void handle_home_display(void) {
    if (touch_in_area(0, 128, 0, 64)) {
        currentState = STATE_MENU;
        touchActive = false;
    }
}

void handle_menu_display(void) {

    if (touch_in_area(4, 27, 14, 60)) {
        touchActive = false;
        currentState = mod_vrata ? STATE_DOOR_MANUAL : (vrata_otvorena_zatvorena ? STATE_DOOR_AUTO_OPEN : STATE_DOOR_AUTO_CLOSED);
        manual_message_printed = false;
        return;
    }

    if (touch_in_area(36, 59, 14, 60)) {
        touchActive = false;
        currentState = STATE_CO2;
        return;
    }

    if (touch_in_area(68, 91, 14, 60)) {
        touchActive = false;
        currentState = STATE_LIGHT;
        return;
    }

    if (touch_in_area(100, 123, 14, 60)) {
        touchActive = false;
        currentState = STATE_HOME;
        return;
    }
}

void handle_manual_door_display(void) {

    if (!manual_message_printed) {
        serial_print_string("Upisite otvori/zatvori u terminal kako bi kontrolisali vrata!\n\r");
        manual_message_printed = true;
    }
    if (touch_in_area(6, 29, 20, 41)) {
        touchActive = false;
        currentState = vrata_otvorena_zatvorena
                ? STATE_DOOR_AUTO_OPEN
                : STATE_DOOR_AUTO_CLOSED;
        mod_vrata = false;
        return;
    }

    if (touch_in_area(48, 71, 20, 41) && !vrata_otvorena_zatvorena) {
        open_doors_manual();
        delay_ms(20);
        vrata_otvorena_zatvorena = true;
        touchActive = false;
        return;
    }

    if (touch_in_area(92, 115, 20, 41) && vrata_otvorena_zatvorena) {
        close_doors();
        delay_ms(20);
        vrata_otvorena_zatvorena = false;
        touchActive = false;
        return;
    }

    if (uart_command_ready) {
        uart_command_ready = false;

        if (strcmp((char*) uart_rx_buffer, "otvori") == 0) {
            serial_print_string("Otvaram vrata...\n\r");
            open_doors_manual();
            delay_ms(20);
            vrata_otvorena_zatvorena = true;
        } else if (strcmp((char*) uart_rx_buffer, "zatvori") == 0) {
            serial_print_string("Zatvaram vrata...\n\r");
            close_doors();
            delay_ms(20);
            vrata_otvorena_zatvorena = false;
        } else {
            serial_print_string("Nepoznata komanda.\n\r");
        }
    }
}

void handle_auto_door_display(void) {

    if (touch_in_area(6, 29, 20, 41)) {
        touchActive = false;
        mod_vrata = true;
        currentState = STATE_DOOR_MANUAL;
        manual_message_printed = false;
        return;
    }
    if (pir_aktiviran() && !pir_reagovao) {
        open_doors();
        delay_ms(20);
        vrata_otvorena_zatvorena = true;
        pir_reagovao = true;
        currentState = STATE_DOOR_AUTO_OPEN;
    } else if (!pir_aktiviran() && pir_reagovao) {
        close_doors();
        delay_ms(20);
        vrata_otvorena_zatvorena = false;
        pir_reagovao = false;
        currentState = STATE_DOOR_AUTO_CLOSED;
    }
}

void handle_co2_display(void) {
    if (!co2_pixels_copied) {
        memcpy(perc_bar, co2_display, 640);
        co2_pixels_copied = true;
        light_pixels_copied = false;
    }
    if (glcd_update) {
        update_percentage_display(perc_bar, scale_co2(mq3));
        glcd_update = false;
    }
    if (serijska) {
        serial_print_string("Procenat co2:\n\r");
        WriteUART1dec2string(scale_co2(mq3));
    }
}

void handle_light_display(void) {
    if (!light_pixels_copied) {
        memcpy(perc_bar, light_display, 640);
        light_pixels_copied = true;
        co2_pixels_copied = false;
    }
    if (glcd_update) {
        update_percentage_display(perc_bar, scale_light(fotootp));
        glcd_update = false;
    }

    if (serijska) {
        serial_print_string("Procenat svjetla:\n\r");
        WriteUART1dec2string(scale_light(fotootp));
    }
}

void handle_reset_to_menu(void) {
    if (touchActive && touch_in_area(110, 128, 48, 64) &&
            currentState != STATE_HOME && currentState != STATE_MENU) {
        if (serijska) {
            serial_print_string("Pritisnuto dugme za nazad, vracam se na meni!\n\r");
        }
        touchActive = false;
        currentState = STATE_MENU;
    }
}
