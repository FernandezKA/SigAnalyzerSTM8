#include "general.h"

uint8_t ucPWM_Measure = 0;
uint16_t usClockCounter = 0;
uint16_t usClockUncapture = 0;
uint16_t usClockUnStop = 0;
uint16_t usSysTick = 0; 
bool bStart = FALSE;
bool bGenFromTable = FALSE;
bool bFirstStart = FALSE;
bool bFirstDetect = FALSE;
bool bFirstPWM = FALSE;
int SystemInit(void)
{
    vClock_Config();
    vTestOut_Config();
    vTim1_Config();
    vTim2_Config();
    vTim4_Config();
    GPIOD->ODR&=~(1<<3);
    vSetPWM1(10);
#ifdef DEBUG
    vUart_Config();
#endif
    return 0;
}

void main(void)
{
	SystemInit();
        asm("RIM");
        bGenFromTable = FALSE;
	 while (1){//Detect new states always
           if(usSysTick == 10000){
             if(ucPWM_Measure > 10){
               vSetPWM1(50);
             }else{
               vSetPWM1(10);
             }
             vTim2_EnablePWM();
           }
           if(usClockUncapture >= 15000){//This case must be call after 5 Sec undetected rise or Edge
                    //bFirstDetect = TRUE;
                    //vTim2_EnablePWM();
                    usClockUncapture = 0;
                    vClearMeasure();
                    vSetPWM1(10);
                    //ucCurrentIndexGen = GEN_SIZE - 1;
           }
           if(usClockUnStop >= 30000){//If stop signal not be receieved after 10 sec - terminate start respond answer
              bStart = FALSE;
              usClockUnStop = 0;  
              //vTim2_DisablePWM();
              bGenFromTable = FALSE;
              GPIOD->ODR&=~(1<<2); 
            }
          
           if(bNewSample){
#ifdef DEBUG
              //Print((uint8_t)(xNewSample.time/(uint16_t) 10));
#endif
              PWMM ePWMCurrent = few_samples;
              State eCurrentState = eGetParse(xNewSample, &ePWMCurrent);
              switch(eCurrentState){
                case start:
                  GPIOD->ODR|=(1<<5);
                  vClearMeasure();
                  //vTim2_DisablePWM();
                  usClockUnStop = 0;
                  bStart = TRUE;
                  bGenFromTable = TRUE;
                break;
                
                case stop:
                  GPIOD->ODR|=(1<<5);
                  vClearMeasure();
                  bFirstStart = TRUE;
                  //vTim2_DisablePWM();
                  bStart = FALSE;
                  usClockUnStop = 0;
                  bGenFromTable = FALSE;
                  ucCurrentIndexGen = GEN_SIZE - 1;
                  GPIOD->ODR&=~(1<<2);
                break;
                
                case pwm:
                  if(ePWMCurrent == detected){
                    GPIOD->ODR|=(1<<5);
                    if(ucPWM_Measure >= 50){
                      vSetPWM1(50);
                    }else{
                      vSetPWM1(10);
                    }
                    //if(!bStart&&ucCountValid == 0){
                    //vTim2_EnablePWM();
                    //}
                  }
                else{
                  if(!bFirstPWM){
                    vSetPWM1(10);
                    bFirstPWM = TRUE;
                  }
                  asm("nop");
                }
                break;
              }
             bNewSample = FALSE;
           }  
         }   
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
