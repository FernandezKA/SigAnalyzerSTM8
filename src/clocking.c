#include "clocking.h"
void vClock_Config(void){
 //CLK->CKDIVR = 0x00;                    // Предделитель равен нулю
 CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
}