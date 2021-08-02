#ifndef _gpio_h_
#define _gpio_h_
#include "general.h"
//This function config GPIO for indicate
void vLED_Config(void);
void vTestOut_Config(void);
void vLedOff(void);
void vLedRun(bool state);
void vLedActive(bool state);
#endif