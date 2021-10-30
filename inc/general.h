
#ifndef _general_h_
#define _general_h_
//#define DEBUG
#include "stdlib.h"
#include "stm8s_conf.h"
#include "stm8s_tim1.h"
#include "stm8s_uart1.h"
#include "stm8s_flash.h"
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
extern volatile uint8_t ucPWM_Measure;
extern volatile uint16_t usClockCounter;
extern volatile uint16_t usClockUncapture;
extern volatile uint16_t usClockUnStop;
extern bool bGenFromTable;
extern bool bStart;
extern bool bFirstStart;
extern bool bFirstDetect;
extern uint16_t u16FirstPWM;
extern uint16_t u16SecondPWM;
extern bool IsAction; 
extern uint8_t u8CountBlinkAction;
void vDetectBoot(void);
#include "sig_generate.h"
#include "pwm_generate.h"
#include "pwm_measure.h"
#include "parser.h"
#include "timers.h"
#include "clocking.h"
#include "gpio.h"
#include "uart.h"
#include "spi.h"
#endif