#include "general.h"
uint8_t ucPWM_Measure = 0;
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
              PWMM ePWMCurrent = few_samples;
              State eCurrentState = eGetParse(xNewSample, &ePWMCurrent);
              switch(eCurrentState){
              case start:
                asm("nop");
                break;
              case stop:
                asm("nop");                
                break;
              case pwm:
                if(ePWMCurrent == detected){
                  asm("nop");
                }
                else{
                  asm("nop");
                }
                break;
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
