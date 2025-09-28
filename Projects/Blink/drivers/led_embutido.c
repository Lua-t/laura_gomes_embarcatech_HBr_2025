#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <stdbool.h>

void ligar(bool estado){
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, estado);
}
bool inicializar(void){
    if (cyw43_arch_init()) {
        return 0; //deu ruim
    }
    return 1;
}
