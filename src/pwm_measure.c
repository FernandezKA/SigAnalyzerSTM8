#include "pwm_measure.h"
uint16_t u16PWMOnes = 0;
uint8_t u8PWMFill = 0;
uint16_t u16CountSamples = 0;
uint8_t u8PWMMeasured = 0;
uint16_t usSummaryTime = 0;
uint16_t usPWMLowTime = 0;
uint16_t usPWMHighTime = 0;
uint16_t u16FirstPWM = 0;
uint16_t u16SecondPWM = 0;
/*
*@brief: this function detect and measure PWM fill after 2 sec.
*@inval: struct type of pulse, variable for writing detected pwm fill
*@retval: enumerate list state of detect
*/
PWMM ePWM_Measure(Pulse pulse, uint8_t volatile* pPWM){
  if(pulse.polarity){
    if(pulse.time < 500){
    usPWMHighTime+=pulse.time;
    }
  }
  else{
    if(pulse.time < 500){
    usPWMLowTime+=pulse.time;
    }
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