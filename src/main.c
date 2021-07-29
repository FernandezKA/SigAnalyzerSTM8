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
        uint8_t PWM_1;
	 while (1){
           if(bNewSample){
             bool bStart = bIsStart(xNewSample);
             bool bStop = bIsStop(xNewSample);
             PWMM ePWM = ePWM_Measure(xNewSample, &PWM_1);
             if(bStart){
               asm("nop");
               bStart = FALSE;
             }
             if(bStop){
               asm("nop");
               bStop = FALSE;
             }
             if(ePWM == detected){
               asm("nop");
               ePWM = few_samples;
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
