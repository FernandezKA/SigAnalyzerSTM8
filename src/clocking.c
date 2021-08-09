#include "clocking.h"
void vClock_Config(void){
 //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
 CLK->ECKR|=CLK_ECKR_HSEEN;
 CLK->SWCR|=CLK_SWCR_SWEN;
 while(((CLK->ECKR & CLK_ECKR_HSERDY) != CLK_ECKR_HSERDY)) {asm("nop");}
 CLK->CKDIVR = 0x00;
 CLK->SWR = 0xB4;        //HSE AS MASTER CLOCK SOURCE
 while((CLK->SWCR & CLK_SWCR_SWIF) != CLK_SWCR_SWIF) {asm("nop");}
}