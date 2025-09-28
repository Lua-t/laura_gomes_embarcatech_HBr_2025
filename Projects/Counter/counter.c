
#include <stdio.h> 
#include "pico/stdlib.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"

#define BUTTON_A 5
#define BUTTON_B 6
#define I2C_SDA 14
#define I2C_SCL 15

volatile bool init = false;
volatile bool update = false;
volatile bool countdown = false;
volatile uint16_t counter=0;
volatile uint16_t clicks=0;

struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};
uint8_t ssd[ssd1306_buffer_length];

void gpioCallback();
void InitCountdown();
void updateDisplay();

void gpioCallback(uint gpio, uint32_t events) {
    if(gpio==BUTTON_A && !countdown)
        init = true;
    else if(gpio==BUTTON_B && countdown) {
        clicks++;
        update = true;
    }
}

void InitCountdown() {
    countdown = true;
    clicks=0;
    counter=9;
    while(counter > 0) {
        updateDisplay();
        sleep_ms(1000);
        counter--;
    }

    countdown = false;
    updateDisplay();
    sleep_ms(5000);
}

void updateDisplay() {
    memset(ssd, 0, sizeof(ssd));
    render_on_display(ssd, &frame_area);

    ssd1306_draw_string(ssd, 5, 0, "Counter:");
    int len = snprintf(NULL, 0, "%d", counter);
    char *result = malloc(len + 1);
    snprintf(result, len + 1, "%d", counter);  
    ssd1306_draw_string(ssd, 5, 8, result);

    ssd1306_draw_string(ssd, 5, 24, "Timer Pressed:");
    len = snprintf(NULL, 0, "%d", clicks);
    result = malloc(len + 1);
    snprintf(result, len + 1, "%d", clicks);  
    ssd1306_draw_string(ssd, 5, 32, result);

    render_on_display(ssd, &frame_area);
}

int main()
{
    stdio_init_all();

    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();
    calculate_render_area_buffer_length(&frame_area);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpioCallback);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

    bool countdownStarted = false;
    while (true) {
        if(init) {
            init=false;
            InitCountdown();
        }
        if(update) {
            update=false;
            updateDisplay();
        }
    }

    return 0;
}
