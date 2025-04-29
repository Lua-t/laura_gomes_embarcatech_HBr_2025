#include "hal_led.h"

int main(){
    while(true){
        hal_led_toggle(1);
        sleep_ms(500);
        hal_led_toggle(0);
        sleep_ms(500);
    }
}