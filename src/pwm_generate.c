#include "pwm_generate.h"
/*
*@brief: this procedure configured timer2 for next using
*/
void vTim2_Config(void){
  TIM2->CR1|=TIM2_CR1_ARPE;//AT THIS MOMENT COUNTER IS DISABLE
  TIM2->CCMR1|=(1<<6|1<<5);//PWM MODE 1
  TIM2->CCER1|=TIM2_CCER1_CC1E;
  TIM2->ARRL = 0x00;
  
}