#include "general.h"

int SystemInit(void)
{
    vClock_Config();
    vTestOut_Config();
    vTim1_Config();
    vTim2_Config();
    vTim4_Config();
    return 0;
}

void main(void)
{
	SystemInit();
        asm("RIM");
	 while (1){
           if(bNewSample){
             Pulse xTemp = xNewSample;
             bool bStart = bIsStart(xTemp);
             if(bStart){
               asm("nop");
             }
             bNewSample = FALSE;
           }
           
           
           
         };   
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
