#ifndef _pwm_measure_h_
#define _pwm_measure_h_
#include "general.h"
enum pwmm{
  few_samples = 0,
  detected = 1
};
typedef enum pwmm PWMM;
PWMM ePWM_Measure(Pulse pulse, uint8_t* pPWM);
#endif