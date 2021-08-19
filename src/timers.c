#include "timers.h"
//variables
enum edge Edge = error;
uint16_t usLowTime = 0;
uint16_t usHighTime = 0;
uint8_t index = 255;
void vTim1_Config(void){
  CLK->PCKENR1|=CLK_PCKENR1_TIM1;//ENABLE CLOCKING
  TIM1->PSCRH = (8000>>8);//set prescaler
  TIM1->PSCRL = 8000&0xFF;
  TIM1->CCMR1|=(1<<0);//CC1 channel is configured as input, IC1 is mapped on TI1FP1
  TIM1->CCMR2|=(1<<1);//CC2 channel is configured as input, IC2 is mapped on TI2FP2
  TIM1->CCER1|=TIM1_CCER1_CC1P;
  TIM1->CCER1|=TIM1_CCER1_CC1E;
  //TIM1->CCER1|=TIM1_CCER1_CC2P;
  TIM1->CCER1|=TIM1_CCER1_CC2E;
  TIM1->IER|=TIM1_IER_CC1IE;
  TIM1->IER|=TIM1_IER_CC2IE;
  TIM1->CR1|=TIM1_CR1_CEN;
  //TIM1_ICInit(TIM1_CHANNEL_1, TIM1_ICPOLARITY_RISING，TIM1_ICSELECTION_DIRECTTI, TIM1_ICPSC_DIV8,0x00);
}
/*
*@brief: this procedure used for PWM generates config
*/
void vTim2_Config(void){
  
  CLK->PCKENR1|=CLK_PCKENR1_TIM2;/*ENABLE CLOCKING*/
  TIM2->PSCR|=(1<<2|1<<1|1<<0);
  TIM2->CR1|=TIM2_CR1_ARPE;//AT THIS MOMENT COUNTER IS DISABLE
  TIM2->ARRH = 244U>>8;
  TIM2->ARRL = 244U&0xFFU;
  TIM2->CCR1H = 24U>>8;
  TIM2->CCR1L = 24U&0xFFU;
  TIM2->CCR2H = 24U>>8;
  TIM2->CCR2L = 24U&0xFFU;
  TIM2->CCR3H = 24U>>8;
  TIM2->CCR3L = 24U&0xFFU;
  //TIM2->CCER1|=TIM2_CCER1_CC2E;/*ENABLE CAPTURE/COMPARE FOR CHANNEL 2 AND 3*/
  //TIM2->CCER2|=TIM2_CCER2_CC3E;
  TIM2->CCMR1|=(1U<<6|1U<<5|1U<<3);/*MODE 1 WITH OUTPUT COMPARE PRELOAD*/
  TIM2->CCMR2|=(1U<<6|1U<<5|1U<<3);/*MODE 1 WITH OUTPUT COMPARE PRELOAD*/
  TIM2->CCMR3|=(1U<<6|1U<<5|1U<<3);/*MODE 1 WITH OUTPUT COMPARE PRELOAD*/
  GPIOD->ODR&=~(1<<3);
  //TIM2->CR1|=TIM2_CR1_CEN;/*RUN TIM2*/
}
void vSetPWM1(uint8_t pwm){
  uint16_t normPWM = (pwm*244UL)/100UL;
  TIM2->CCR2H = normPWM>>8;
  TIM2->CCR2L = normPWM&0xFFU;
}

void vTim4_Config(void){
  /*This timer using for definition frequency of sampling*/
  CLK->PCKENR1 |= CLK_PCKENR1_TIM4;
  TIM4->IER |= TIM4_IER_UIE; /*ENABLE INTERRUPT UPDATE*/
  TIM4->IER |= TIM4_IER_RESET_VALUE;
  TIM4->PSCR = (1<<2|1<<1|1<<0); /*SET PRESCALER = 2^15*/
  TIM4->ARR = 0x3EU;//7C for sampling
  ITC->ISPR6 &= 0;
  ITC->ISPR6 |= 0x03U;
  TIM4->SR1 = ~TIM4_SR1_UIF; /*clear uif bit at SREG for correct working*/
  TIM4->CR1 |= TIM4_CR1_CEN;
}
/********************************IRQ Section***********************************/
/*
*@brief: this IRQ handler used for Input Capture for request sequence detection and PWM_Measure detect
*/
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  uint8_t ReadSR1Reg = TIM1->SR1;//CLAEAR AFTER READING REGISTERS
  usClockUncapture = 0;
  TIM1->SR1&=~TIM1_SR1_CC1IF;
  TIM1->SR1&=~TIM1_SR1_CC2IF;
  if((ReadSR1Reg&TIM1_SR1_CC1IF)==TIM1_SR1_CC1IF){
    Edge = fall;
  }
  else if((ReadSR1Reg&TIM1_SR1_CC2IF) == TIM1_SR1_CC2IF){
    Edge = rise;
  }
  else{
    Edge = error;
  }
  switch(Edge){
  case rise:
      TIM1->CNTRH = 0x00;
      TIM1->CNTRL = 0x00;
     usLowTime = TIM1->CCR2L;
     usLowTime |= (TIM1->CCR2H)<<8;
     xNewSample.time = usLowTime;
     xNewSample.polarity = FALSE;
     bNewSample = TRUE;
    break;
  case fall:
     TIM1->CNTRH = 0x00;
     TIM1->CNTRL = 0x00;
     usHighTime = TIM1->CCR1L;
     usHighTime |= (TIM1->CCR1H)<<8; 
     xNewSample.time = usHighTime;
     xNewSample.polarity = TRUE;
     bNewSample = TRUE;
    break;
  case error:
    xNewSample.time = 0;
    xNewSample.polarity = FALSE;
    bNewSample = FALSE;
    break;
  }
}
////////////////////////////
/*
*@brief: this timer used for define sampling 1 mS 
*/
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
  TIM4->SR1 &= (uint8_t) ~(TIM4_SR1_UIF);//Clear status register for out from IRQ
  //GPIOD->ODR^=(1<<4);
  if(usSysTick%6000 == 0){
  _AD8400_set(--index); 
  }
  ++usSysTick;
  if(usSysTick%500 == 0){
    GPIOD->ODR^=(1<<4);
    GPIOD->ODR&=~(1<<5);
  }
  /*********************************/
  ++usClockUncapture;
  /*********************************/
  if(bStart){
    usClockUnStop++;
  }
  if(bGenFromTable){
      ++usCurrentIndexSample;
   if(usCurrentIndexSample < sigGen[ucCurrentIndexGen].time){
     asm("nop");
   }
   else{//update sample time
    usCurrentIndexSample = 0;
    vIncrementGenIndex(&ucCurrentIndexGen);
    if(sigGen[ucCurrentIndexGen].polarity){
      GPIOD->ODR&=~(1<<2);
    }
    else{
       GPIOD->ODR|=(1<<2);
    }
  }
}
  //This counter must overflow after 0xFFFF, 65535 mS = ~65.5 Sec
}
