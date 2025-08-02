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

        //provjerava promjenu stanja i update-uje display
        check_state();
        //poziva odgovarajucu f-ju za konkretni meni
        handle_state();
        //provjerava povratak na meni
        handle_reset_to_menu();
        //pali/gasi svjetla
        update_lights();

    }//while

    return 0;
}//main




