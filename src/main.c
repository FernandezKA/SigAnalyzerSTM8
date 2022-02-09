#include "general.h"
//#define TEST
//#define BOOTLOADER
/*Block of variables*/
//Useless
volatile uint8_t ucPWM_Measure = 0;
//Usefull
volatile uint16_t usClockCounter = 0;
volatile uint16_t usClockUncapture = 0;
volatile uint16_t usClockUnStop = 0;
volatile uint16_t usSysTick = 0; 
bool bStart = FALSE;
bool bGenFromTable = FALSE;
bool bFirstStart = FALSE;
bool bFirstDetect = FALSE;
bool bFirstPWM = FALSE;
uint32_t* pOptionBytes = (uint32_t*) 0x00004800;
void vConfigOptionBytes(void){
  FLASH_DeInit();  
  FLASH_Unlock(FLASH_MEMTYPE_DATA); 
  //*(pOptionBytes + 0x03) = 0x01;
  //*(pOptionBytes + 0x04) = 0xFE;
  FLASH_ProgramOptionByte(0x4803, (1<<0));
  //FLASH_ProgramOptionByte(0x4804, 0xFE);
#ifndef TEST
  FLASH_ProgramOptionByte(0x4800, 0xAA);
#endif
  FLASH_Lock (FLASH_MEMTYPE_DATA);
}
//Block of function definition
int SystemInit(void)
{
    vClock_Config();
    vConfigOptionBytes();
    vTestOut_Config();
    vTim1_Config();
    vTim2_Config();
    vTim4_Config();
    GPIOD->ODR&=~(1<<3);
    GPIOC->DDR&=~(1<<7);
    GPIOC->CR1|=(1<<7);
    GPIOC->CR2&=~(1<<7);
    vSetPWM1(10);
#ifdef DEBUG
    vUart_Config();
#endif
    return 0;
}
bool IsAction = FALSE;
uint8_t u8CountBlinkAction = 0;
void main(void)
{
	SystemInit();
        bGenFromTable = FALSE;
        asm("RIM");
#ifdef BOOTLOADER
        asm("trap");
#endif
	 while (1){//Detect new states always into the loop
           //This is a statical function, define rules at time
           if(usSysTick == 10000UL){//After 10 second at power on, enable PWM
            vTim2_EnablePWM(); 
           }
           if(usClockUncapture >= 15000UL){//This case must be call after 15 Sec undetected rise or Edge
                    //bFirstDetect = TRUE;
                    //vTim2_EnablePWM();
                    usClockUncapture = 0;
                    vClearMeasure();
                    vSetPWM1(10U);
                    //ucCurrentIndexGen = GEN_SIZE - 1;
           }
           if(usClockUnStop >= 30000UL){//If stop signal not be receieved after 30 sec - terminate start respond answer
              bStart = FALSE;
              usClockUnStop = 0;  
              bGenFromTable = FALSE;
              GPIOD->ODR&=~(1U<<2); 
            }
           if(u16PWMOnes == 2000U){
             u8PWMFill = 9U; 
             vSetPWM1(10U);
           }
           //This part of code for Input Capture IRQ
           if(bNewSample){//This is true if be input capture IRQ
              //PWMM ePWMCurrent = few_samples;//This case will be need for input capture pwm fill 
              State eCurrentState = eGetParse(xNewSample);//This is example for detect FSM
              switch(eCurrentState){
                case start:
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
                    if(u8PWMFill < 12U){//If PWM fill less than 12%, set PWM 12%
                      vSetPWM1(10U);
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
