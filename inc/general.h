#ifndef _general_h_
#define _general_h_
#include "stdlib.h"
#include "stm8s_conf.h"
#include "stm8s_tim1.h"
//PORT DEFINITION
#define SIG_PORT GPIOD
#define SIG_OUT  (1<<2)
#define PWM_OUT  
#define SIG_INPUT_PORT  GPIOC
#define SIG_INPUT       GPIO_Pin_6
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
extern uint16_t usClockCounter;
extern uint16_t usClockUncapture;
extern bool bGenFromTable;
#include "sig_generate.h"
#include "pwm_generate.h"
#include "pwm_measure.h"
#include "parser.h"
#include "timers.h"
#include "clocking.h"
#include "gpio.h"

#endif