#ifndef _parser_h_
#define _parser_h_
#include "general.h"
struct pulse{
  uint16_t time;
  bool polarity;
};
typedef struct pulse Pulse;
extern Pulse xNewSample;
extern bool bNewSample;
bool bIsStart(Pulse pulse);
bool bIsStop(Pulse pulse);
#endif