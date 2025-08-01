#include "adc.h"
#include "touchscreen.h"
#include "timers.h"
#include "state_machine.h"

_FOSC(CSW_FSCM_OFF & HS3_PLL4);
_FWDT(WDT_OFF);
_FGS(CODE_PROT_OFF);

bool serijska = true;

int main(void) {
    
    //pinovi
    init_pins();
    
    //tajmeri
    configure_timers();
    
    //moduli
    configure_modules();

    while (1) {
        
        check_state();

        switch (currentState) {
            case STATE_HOME: handle_home_display();
                break;
            case STATE_MENU: handle_menu_display();
                break;
            case STATE_VRATA_MANUAL: handle_manual_door_display();
                break;
            case STATE_VRATA_AUTO_OPEN:
            case STATE_VRATA_AUTO_CLOSED:
                handle_auto_door_display();
                break;
            case STATE_CO2: handle_co2_display();
                break;
            case STATE_LIGHT: handle_light_display();
                break;
            default: handle_home_display();
        }
        
        handle_reset_to_menu();
        update_lights();
        
    }//while

    return 0;
}//main




