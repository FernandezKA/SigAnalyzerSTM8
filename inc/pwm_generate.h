#ifndef _pwm_generate_h_
#define _pwm_generate_h_
#include "general.h"
void vTim2_Config(void);
void vTim2_SetFill(uint8_t fill);
void vTim2_EnablePWM(void);
void vTim2_DisablePWM(void);
#endif