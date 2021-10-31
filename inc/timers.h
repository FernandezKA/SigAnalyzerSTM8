#ifndef _timers_h_
#define _timers_h_
#include "general.h"
enum edge{
  rise = 0, 
  fall=1, 
  error = 2
};
extern enum edge Edge;
extern uint16_t usLowTime;
extern uint16_t usHighTime;
extern volatile uint16_t usSysTick;
void vTim1_Config(void);//This timer used for input capture
void vTim2_Config(void);//This timer used for PWM generate
void vTim4_Config(void);//This timer used for definition freqency of sampling
void vSetPWM1(uint8_t pwm);
#endif