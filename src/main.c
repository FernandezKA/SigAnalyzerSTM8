#include "general.h"
#include "uart.h"
#define TEST
//#define BOOTLOADER
/*Block of variables*/

static inline void send(volatile uint8_t data){
  while((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE) {asm("nop");}
  UART1->DR = data;
  return;
}

static inline void print(volatile uint8_t* data,volatile uint8_t size){
  for(uint8_t i = 0; i < size; ++i){
    send(data[i]);
  }
}

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
void vConfigOptionBytes(void){
  FLASH_DeInit();  
  FLASH_Unlock(FLASH_MEMTYPE_DATA); 
  FLASH_ProgramOptionByte(0x4803, (1<<0));
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
    vUart_Config();
    vSetPWM1(10);
#ifdef DEBUG
    vUart_Config();
#endif
    return 0;
}

static uint8_t countPWM = 0;

bool IsAction = FALSE;
uint8_t u8CountBlinkAction = 0;
void main(void)
{
	SystemInit();
        //UART1->DR = 0x55;
        //send(0x55U);
        print("Pump_1_2\n\r", 10);
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
                    vTim2_EnablePWM();
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
                  countPWM = 0;
                  print("Start\n\r", 7);
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
                  countPWM = 0;
                  print("Stop\n\r", 6);
                  vClearMeasure();
                  bFirstStart = TRUE;
                  bStart = FALSE;
                  usClockUnStop = 0;
                  bGenFromTable = FALSE;
                  ucCurrentIndexGen = GEN_SIZE - 1;
                  GPIOD->ODR&=~(1<<2);//Pull up sig gen out
                  u8PWMMeasured = 0;
                  u8PWMFill = 9;
                  //u16PWMOnes = 0; 
                  //u16CountSamples = 0;
                  vSetPWM1(10);
                break;
                
                case pwm:
                  print("PWM\n\r", 5);
                  if(countPWM < 0x0A) {//Check valid PWM
                    ++countPWM;
                  }
                  else{//PWM already detected
                    if(u8PWMMeasured > 0){
                      --u8PWMMeasured;//This is indicate of measured PWM
                      vSetPWM2(u8PWMFill);
                      TIM2->CCER2|=TIM2_CCER2_CC3E;
                    if(u8PWMFill < 12U){//If PWM fill less than 12%, set PWM 12%
                      vSetPWM1(10U);
                    }
                    else{//If PWM more than 12%, set PWM fill 50%
                      vSetPWM1(50);
                      }
                    }
                  }

                break;
                
              case mistake:
                  print("MST\n\r", 5);
                  u8PWMMeasured = 0;
                  //u8PWMFill = 9;
                  //u16PWMOnes = 0; 
                  //u16CountSamples = 0;
                  //vSetPWM1(10);
//                  xNewSample.time = 0;
//                  xNewSample.polarity = TRUE;
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
