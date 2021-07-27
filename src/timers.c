#include "timers.h"
void vTim1_Config(void){
  CLK->PCKENR1|=CLK_PCKENR1_TIM1;//ENABLE CLOCKING
  TIM1->CR1|=TIM1_CR1_CEN;
  TIM1->CCMR1|=(1<<0);//CC1 channel is configured as input, IC1 is mapped on TI1FP1
  TIM1->CCER1|=TIM1_CCER1_CC1E;
  TIM1->BKR|=TIM1_BKR_MOE;
  TIM1->IER|=TIM1_IER_CC1IE;
}
/*
*@brief: this procedure used for PWM generate config
*/
void vTim2_Config(void){
  
  CLK->PCKENR1|=CLK_PCKENR1_TIM2;/*ENABLE CLOCKING*/
  TIM2->PSCR|=(1<<2|1<<1|1<<0);
  TIM2->CR1|=TIM2_CR1_ARPE;//AT THIS MOMENT COUNTER IS DISABLE
  TIM2->ARRH = 488U>>8;
  TIM2->ARRL = 488U&0xFFU;
  TIM2->CCR1H = 244U>>8;
  TIM2->CCR1L = 244U&0xFFU;
  TIM2->CCR2H = 244U>>8;
  TIM2->CCR2L = 244U&0xFFU;
  TIM2->CCR3H = 244U>>8;
  TIM2->CCR3L = 244U&0xFFU;
  TIM2->CCER1|=TIM2_CCER1_CC2E;/*ENABLE CAPTURE/COMPARE FOR CHANNEL 2 AND 3*/
  TIM2->CCER2|=TIM2_CCER2_CC3E;
  TIM2->CCMR1|=(1U<<6|1U<<5|1U<<3);/*MODE 1 WITH OUTPUT COMPARE PRELOAD*/
  TIM2->CCMR2|=(1U<<6|1U<<5|1U<<3);/*MODE 1 WITH OUTPUT COMPARE PRELOAD*/
  TIM2->CCMR3|=(1U<<6|1U<<5|1U<<3);/*MODE 1 WITH OUTPUT COMPARE PRELOAD*/
  TIM2->CR1|=TIM2_CR1_CEN;/*RUN TIM2*/
}
/*
*@brief: this procedure used for generate from table
*/
void vTim3_Config(void){
  
}
void vTim4_Config(void){
  /*This timer using for definition frequency of sampling*/
  CLK->PCKENR1 |= CLK_PCKENR1_TIM4;
  TIM4->IER |= TIM4_IER_UIE; /*ENABLE INTERRUPT UPDATE*/
  TIM4->IER |= TIM4_IER_RESET_VALUE;
  TIM4->PSCR = (1<<2|1<<1|1<<0); /*SET PRESCALER = 2^15*/
  TIM4->ARR = 0x1EU;
  ITC->ISPR6 &= 0;
  ITC->ISPR6 |= 0x03U;
  TIM4->SR1 = ~TIM4_SR1_UIF; /*clear uif bit at SREG for correct working*/
  TIM4->CR1 |= TIM4_CR1_CEN;
}
/*
*@brief: this IRQ handler used for definition frequency of sampling
*/
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
  TIM4->SR1 &= (uint8_t) ~(TIM4_SR1_UIF);//Clear status register for out from IRQ
  GPIOD->ODR^=(1<<2);//This string for testing frequensy of sampling
}

INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  //TIM1->SR1&=~TIM1_SR1_CC1IF;
        asm("nop");
}