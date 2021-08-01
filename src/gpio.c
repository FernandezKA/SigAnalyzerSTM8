#include "gpio.h"
void vLED_Config(void){
  
}
void vTestOut_Config(void){
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);//run
  GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);//stop
  GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);//pwm
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT );
  GPIOC->CR1|=(1<<6);
}
void vLedOff(void){
  GPIOD->ODR&=~(1<<4|1<<5|1<<6);
}
void vLedRun(bool state){
  if(state){
  GPIOD->ODR|=(1<<4);
  }else{
    GPIOD->ODR&=~(1<<4);
  }
}
void vLedStop(bool state){
  if(state){
  GPIOD->ODR|=(1<<5);
  }else{
    GPIOD->ODR&=~(1<<5);
  }
}

void vLedPWM(bool state){
  if(state){
  GPIOD->ODR|=(1<<6);
  }else{
    GPIOD->ODR&=~(1<<6);
  }
}