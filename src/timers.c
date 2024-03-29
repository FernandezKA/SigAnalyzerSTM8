#include "timers.h"
//variables
enum edge Edge = error;
uint16_t usLowTime = 0;
uint16_t usHighTime = 0;
volatile uint8_t index = 255;
uint16_t u8PartIndexInc = 0x00;
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
//  TIM2->ARRH = 244U>>8;
//  TIM2->ARRL = 244U&0xFFU;
  TIM2->ARRH = 625U>>8;
  TIM2->ARRL = 625U&0xFFU;
  TIM2->CCR1H = 63U>>8;
  TIM2->CCR1L = 63U&0xFFU;
  TIM2->CCR2H = 63U>>8;
  TIM2->CCR2L = 63U&0xFFU;
  TIM2->CCR3H = 63U>>8;
  TIM2->CCR3L = 63U&0xFFU;
  //TIM2->CCER1|=TIM2_CCER1_CC2E;/*ENABLE CAPTURE/COMPARE FOR CHANNEL 2 AND 3*/
  TIM2->CCER2|=TIM2_CCER2_CC3P;
  TIM2->CCMR1|=(1U<<6|1U<<5|1U<<3);/*MODE 1 WITH OUTPUT COMPARE PRELOAD*/
  TIM2->CCMR2|=(1U<<6|1U<<5|1U<<3);/*MODE 1 WITH OUTPUT COMPARE PRELOAD*/
  TIM2->CCMR3|=(1U<<6|1U<<5|1U<<3);/*MODE 1 WITH OUTPUT COMPARE PRELOAD*/
  GPIOD->ODR&=~(1<<3);
  //TIM2->CR1|=TIM2_CR1_CEN;/*RUN TIM2*/
}
void vSetPWM1(uint8_t pwm){
  uint16_t normPWM = (pwm*625UL)/100UL;
  TIM2->CCR2H = normPWM>>8;
  TIM2->CCR2L = normPWM&0xFFU;
}

void vSetPWM2(volatile uint8_t pwm2){
  uint16_t normPWM = (pwm*625UL)/100UL;
  TIM2->CCR3H = (uint8_t) normPWM>>8;
  TIM2->CCR3L = (uint8_t) normPWM&0xFFU;
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
  volatile uint8_t ReadSR1Reg = TIM1->SR1;//CLAEAR AFTER READING REGISTERS
  usClockUncapture = 0;
  TIM1->SR1&=~TIM1_SR1_CC1IF;
  TIM1->SR1&=~TIM1_SR1_CC2IF;
  IsAction = TRUE;
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
  if(u16CountSamples == 2000UL){//At this case we recognize PWM filling
    ++u8PWMMeasured;
    u8PWMFill = u16PWMOnes / 20;
    u16PWMOnes = 0; 
    u16CountSamples = 0;
    if(u8PWMFill > 98){//If line is high state all of time, set default PWM fill
      u8PWMFill = 10;
    }
  }
  else{//A few samples for recognize
    if((GPIOC->IDR & (1<<6)) == (1<<6)){//At this case we increment ones value of sample
     ++u16PWMOnes; 
     ++u16CountSamples;
    }
    else{//At this case we only increment index, without ones 
      ++u16CountSamples;
    }
  }
  if(usSysTick%7000 == 0){//Every 6 second increment value of AD8400
    ++u8PartIndexInc; 
    if(u8PartIndexInc==100){
      u8PartIndexInc = 0;
      if(index == 0x00){
        asm("nop");
      }
      else{
      _AD8400_set(--index);  
      }
    }
  }
  ++usSysTick;
  if(usSysTick == 60000UL){
    usSysTick = 0;
  }
  if(usSysTick%250 == 0){//This case define for led blinking
    GPIOD->ODR^=(1<<4);
    if(IsAction){
      if(u8CountBlinkAction < 6){
        ++u8CountBlinkAction;
        GPIOC->ODR^=(1<<7);
      }
      else{
        IsAction = FALSE;
        u8CountBlinkAction = 0;
        GPIOC->ODR&=~(1<<7);
      }
    }
    //GPIOD->ODR^=(1<<5);
  }
  /*********************************/
  ++usClockUncapture;
  /*********************************/
  if(bStart){//Start count of unstoppped signal
    usClockUnStop++;
  }
  if(bGenFromTable){//This case define for generate from table
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
  //TODO make overflow at 600000 samples
}
