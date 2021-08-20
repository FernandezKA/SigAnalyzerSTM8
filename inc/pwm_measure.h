#ifndef _pwm_measure_h_
#define _pwm_measure_h_
#include "general.h"
extern uint16_t u16PWMOnes;
extern uint8_t u8PWMFill;
extern uint16_t u16CountSamples; 
extern uint8_t u8PWMMeasured;
PWMM ePWM_Measure(Pulse pulse, uint8_t* pPWM);
void vClearMeasure(void);
#endif