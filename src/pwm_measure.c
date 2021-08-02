#include "pwm_measure.h"
uint16_t usSummaryTime = 0;
uint16_t usPWMLowTime = 0;
uint16_t usPWMHighTime = 0;
/*
*@brief: this function detect and measure PWM fill after 2 sec.
*@inval: struct type of pulse, variable for writing detected pwm fill
*@retval: enumerate list state of detect
*/
PWMM ePWM_Measure(Pulse pulse, uint8_t* pPWM){
  if(pulse.polarity){
    usPWMHighTime+=pulse.time;
  }
  else{
    usPWMLowTime+=pulse.time;
  }
  usSummaryTime = usPWMLowTime + usPWMHighTime;
  if(usSummaryTime >= 2000){
    usSummaryTime = usSummaryTime/100;
    *pPWM = usPWMHighTime/usSummaryTime;
    usSummaryTime = 0;
    usPWMLowTime = 0;
    usPWMHighTime = 0;
    return detected;
  }
  else{
    return few_samples;
  }
}
/*
*@brief: This function clear all measured value
*/
void vClearMeasure(void){
    usSummaryTime = 0;
    usPWMLowTime = 0;
    usPWMHighTime = 0;
}