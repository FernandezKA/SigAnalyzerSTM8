#ifndef _general_h_
#define _general_h_
#include "stdlib.h"
#include "stm8s_conf.h"
#include "stm8s_tim1.h"
struct pulse{
  uint16_t time;
  bool polarity;
};
typedef struct pulse Pulse;
enum pwmm{
  few_samples = 0,
  detected = 1
};
typedef enum pwmm PWMM;
extern uint8_t ucPWM_Measure;
#include "pwm_measure.h"
#include "parser.h"
#include "timers.h"
#include "clocking.h"
#include "gpio.h"

#endif