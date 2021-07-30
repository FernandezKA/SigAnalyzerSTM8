#include "pwm_generate.h"
void vTim2_DisablePWM(void){
  TIM2->CR1&=~TIM2_CR1_CEN;/*RUN TIM2*/
}
void vTim2_EnablePWM(void){
  TIM2->CR1|=TIM2_CR1_CEN;/*RUN TIM2*/
}