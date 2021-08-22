  #include "gpio.h"
#include "spi.h"
void vLED_Config(void){
  
}
void vTestOut_Config(void){
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);//run
  GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);//stop
  GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);//pwm
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT );
  GPIO_Init(SPI_PORT, SPI_NSS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SPI_PORT, SPI_SDI, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SPI_PORT, SPI_SCK, GPIO_MODE_OUT_PP_LOW_FAST);
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
void vLedActive(bool state){
  if(state){
  GPIOD->ODR|=(1<<5);
  }else{
    GPIOD->ODR&=~(1<<5);
  }
}
