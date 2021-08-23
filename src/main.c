#include "general.h"
/*Block of variables*/
//Useless
uint8_t ucPWM_Measure = 0;
//Usefull
uint16_t usClockCounter = 0;
uint16_t usClockUncapture = 0;
uint16_t usClockUnStop = 0;
uint16_t usSysTick = 0; 
bool bStart = FALSE;
bool bGenFromTable = FALSE;
bool bFirstStart = FALSE;
bool bFirstDetect = FALSE;
bool bFirstPWM = FALSE;
//Block of function definition
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
        bGenFromTable = FALSE;
        asm("RIM");
	 while (1){//Detect new states always into the loop
           //This is a statical function, define rules at time
           if(usSysTick == 10000UL){//After 10 second at power on, enable PWM
            vTim2_EnablePWM(); 
           }
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
              bGenFromTable = FALSE;
              GPIOD->ODR&=~(1<<2); 
            }
           if(u16PWMOnes == 2000){
             u8PWMFill = 9; 
             vSetPWM1(10);
           }
           //This part of code for Input Capture IRQ
           if(bNewSample){//This is true if be input capture IRQ
              //PWMM ePWMCurrent = few_samples;//This case will be need for input capture pwm fill 
              State eCurrentState = eGetParse(xNewSample);//This is example for detect FSM
              switch(eCurrentState){
                case start:
                  GPIOD->ODR|=(1<<5);//Enable led action
                  vClearMeasure();
                  usClockUnStop = 0;
                  bStart = TRUE;
                  bGenFromTable = TRUE;
                  u8PWMMeasured = 0;
                  u8PWMFill = 9;
                  u16PWMOnes = 0; 
                  u16CountSamples = 0;
                  vSetPWM1(10);
                break;
                
                case stop:
                  GPIOD->ODR|=(1<<5);//Enable led action
                  vClearMeasure();
                  bFirstStart = TRUE;
                  bStart = FALSE;
                  usClockUnStop = 0;
                  bGenFromTable = FALSE;
                  ucCurrentIndexGen = GEN_SIZE - 1;
                  GPIOD->ODR&=~(1<<2);//Pull up sig gen out
                  u8PWMMeasured = 0;
                  u8PWMFill = 9;
                  u16PWMOnes = 0; 
                  u16CountSamples = 0;
                  vSetPWM1(10);
                break;
                
                case pwm:
                  if(u8PWMMeasured > 0){
                    --u8PWMMeasured;//This is indicate of measured PWM
                    if(u8PWMFill < 12){//If PWM fill less than 12%, set PWM 12%
                      vSetPWM1(10);
                    }
                    else{//If PWM more than 12%, set PWM fill 50%
                      vSetPWM1(50);
                    }
                  }
                break;
                
              case mistake:
                  u8PWMMeasured = 0;
                  u8PWMFill = 9;
                  u16PWMOnes = 0; 
                  u16CountSamples = 0;
                  vSetPWM1(10);
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
