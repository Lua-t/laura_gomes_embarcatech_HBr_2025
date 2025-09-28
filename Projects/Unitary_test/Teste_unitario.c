#include "Teste_unitario.h"

float adc_to_celsius(uint16_t adc_value){
    const float conversion = 3.3f / (1 << 12);
    float voltage = adc_value * conversion;
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;
    return temperature;
}
