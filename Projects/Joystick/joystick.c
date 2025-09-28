#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"


const int VRX = 26;          
const int VRY = 27;         
const int PIN_SW = 22;           

const uint I2C_SDA_PIN = 14;
const uint I2C_SCL_PIN = 15;

void setup_joystick() {
    adc_init();
    adc_gpio_init(VRX);
    adc_gpio_init(VRY);

    gpio_init(PIN_SW);
    gpio_set_dir(PIN_SW, GPIO_IN);
    gpio_pull_up(PIN_SW);
}

void setup_i2c_display() {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    ssd1306_init();
}

uint16_t read_joystick_axis(uint channel) {
    adc_select_input(channel);
    return adc_read();
}

void show_values_on_display(uint16_t x, uint16_t y) {
    struct render_area area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&area);

    uint8_t buffer[ssd1306_buffer_length];
    memset(buffer, 0, sizeof(buffer));

    char message[24];
    snprintf(message, sizeof(message), "X: %4d Y: %4d", x, y);

    ssd1306_draw_string(buffer, 5, 16, message);
    render_on_display(buffer, &area);
}

int main() {
    stdio_init_all();
    sleep_ms(1000);  // Aguarda conexão serial

    setup_joystick();
    setup_i2c_display();

    while (1) {
        uint16_t y_val = read_joystick_axis(0); // VRY no ADC0
        uint16_t x_val = read_joystick_axis(1); // VRX no ADC1

        show_values_on_display(x_val, y_val);
        sleep_ms(500);
    }

    return 0;
}
 



















































