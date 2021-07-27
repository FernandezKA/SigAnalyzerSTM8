#include "gpio.h"
void vLED_Config(void){
  
}
void vTestOut_Config(void){
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
}