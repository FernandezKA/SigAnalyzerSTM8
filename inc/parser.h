#ifndef _parser_h_
#define _parser_h_
#include "general.h"
enum state{
  start = 0, 
  stop = 1, 
  pwm = 3
};
typedef enum state State;
extern State eCurrentState;
extern Pulse xNewSample;
extern bool bNewSample;
extern uint8_t ucCountValid;
State eGetParse(Pulse pulse, PWMM* PWM);
#endif