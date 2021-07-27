#include "gpio.h"
void vLED_Config(void){
  
}
void vTestOut_Config(void){
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT );
}