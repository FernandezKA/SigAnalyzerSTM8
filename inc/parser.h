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
bool bIsStart(Pulse pulse);
bool bIsStop(Pulse pulse);
State eGetParse(Pulse pulse);
#endif