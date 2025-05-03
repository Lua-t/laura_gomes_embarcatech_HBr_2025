#include "unity.h"
#include "Teste_unitario.h"

void tearDown(void){

}
void setUp(void){

}
void test_adc_to_temperature(void)
{
TEST_ASSERT_FLOAT_WITHIN(0.3f,27, adc_to_celsius(876));

}

int main(void)
{
UNITY_BEGIN();
RUN_TEST(test_adc_to_temperature);
return UNITY_END();
}
