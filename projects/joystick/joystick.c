#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

const int VRX = 26;          // Pino de leitura do eixo X do joystick (conectado ao ADC)
const int VRY = 27;          // Pino de leitura do eixo Y do joystick (conectado ao ADC)
const int ADC_CHANNEL_0 = 0; // Canal ADC para o eixo X do joystick
const int ADC_CHANNEL_1 = 1; // Canal ADC para o eixo Y do joystick
const int SW = 22;           // Pino de leitura do botão do joystick
void init_joystick() {
    adc_init();

    // Ativa os pinos ADC
    adc_gpio_init(VRX);  // GPIO26 - ADC0
    adc_gpio_init(VRY);  // GPIO27 - ADC1

    // Inicializa o botão como entrada com pull-up
    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);
}

uint16_t read_adc_channel(uint8_t channel) {
    adc_select_input(channel);
    return adc_read();  // 0 - 4095 (12 bits)
}

int main() {
    stdio_init_all();
    sleep_ms(2000); // Tempo para abrir o terminal

    init_joystick();

    while (true) {
        uint16_t x_value = read_adc_channel(0); // ADC0 - X
        uint16_t y_value = read_adc_channel(1); // ADC1 - Y

        bool btn_pressed = !gpio_get(SW); // Pressionado = LOW

        printf("Joystick X: %d | Y: %d | Botão: %s\n",
               x_value, y_value,
               btn_pressed ? "Pressionado" : "Solto");

        sleep_ms(300);
    }

    return 0;
}

