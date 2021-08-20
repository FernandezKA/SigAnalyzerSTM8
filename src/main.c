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
	 while (1){//Detect new states always into the loop
           if(usSysTick == 10000UL){
            vTim2_EnablePWM(); 
           }
          /* if(usSysTick == 10000){//After 10 second enable PWM
             if(ucPWM_Measure > 10){
               vSetPWM1(50);
             }else{
               vSetPWM1(10);
             }
             vTim2_EnablePWM();
           }*/
           if(usClockUncapture >= 15000){//This case must be call after 15 Sec undetected rise or Edge
                    //bFirstDetect = TRUE;
                    //vTim2_EnablePWM();
                    usClockUncapture = 0;
                    vClearMeasure();
                    vSetPWM1(10);
                    //ucCurrentIndexGen = GEN_SIZE - 1;
           }
           if(usClockUnStop >= 30000){//If stop signal not be receieved after 30 sec - terminate start respond answer
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
              //PWMM ePWMCurrent = few_samples;
              State eCurrentState = eGetParse(xNewSample);
              switch(eCurrentState){
                case start:
                  GPIOD->ODR|=(1<<5);
                  vClearMeasure();
                  usClockUnStop = 0;
                  bStart = TRUE;
                  bGenFromTable = TRUE;
                break;
                
                case stop:
                  GPIOD->ODR|=(1<<5);//???
                  vClearMeasure();
                  bFirstStart = TRUE;
                  bStart = FALSE;
                  usClockUnStop = 0;
                  bGenFromTable = FALSE;
                  ucCurrentIndexGen = GEN_SIZE - 1;
                  GPIOD->ODR&=~(1<<2);//???
                break;
                
                case pwm:
                  if(u8PWMMeasured > 0){
                    --u8PWMMeasured;
                    if(u8PWMFill < 10){//If PWM fill less than 10%, set PWM 10%
                      vSetPWM1(10);
                      //vTim2_EnablePWM();
                    }
                    else{//If PWM more than 10%, set PWM fill 50%
                      vSetPWM1(50);
                      //vTim2_EnablePWM();
                    }
                  }
                   /*if(ePWMCurrent == detected){
                    GPIOD->ODR|=(1<<5);
                    if(ucPWM_Measure >= 50){
                      vSetPWM1(50);
                    }else{
                      vSetPWM1(10);
                    }
                  }*/
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
