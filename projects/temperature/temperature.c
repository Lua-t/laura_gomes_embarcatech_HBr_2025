#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#define adc_temperature_channel 4

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

float adc_to_temperature(uint16_t adc_value)
{
  const float conversion = 3.3f / (1 << 12);
  float voltage = adc_value * conversion;
  float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;
  return temperature;
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
  adc_init();

  adc_set_temp_sensor_enabled(true);
  adc_select_input(adc_temperature_channel);

  struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
  };

  calculate_render_area_buffer_length(&frame_area);

  uint8_t ssd[ssd1306_buffer_length];
  memset(ssd, 0, ssd1306_buffer_length);
  render_on_display(ssd, &frame_area);

  while (true)
  {
    uint16_t adc_value = adc_read();
    float temperature = adc_to_temperature(adc_value);
    int len = snprintf(NULL, 0, "Temperatura: %.2f °C", temperature);
    char *result = malloc(len + 1);
    snprintf(result, len + 1, "%.2f", temperature);
    sleep_ms(5000);
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
    ssd1306_draw_string(ssd, 5, 0, "Temperature: ");
    ssd1306_draw_string(ssd, 5, 8, result);
    ssd1306_draw_string(ssd, 53, 8, "°C");
    render_on_display(ssd, &frame_area);
  }
  return 0;
}