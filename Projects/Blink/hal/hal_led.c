#include "led_embutido.h"

bool inicializado=false;
void hal_led_toggle(bool estado){
    if(!inicializado){
        inicializado=inicializar();
    }
    ligar(estado);
}