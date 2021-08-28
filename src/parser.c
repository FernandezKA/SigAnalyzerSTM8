#include "parser.h"
Pulse xNewSample = {0, FALSE};
bool bNewSample = TRUE;
State eCurrentState = mistake;
uint8_t ucCountValid = 0;
/*
*@brief: this function detect start sequence
*@inval: struct type Pulse
*@retval: bool value of detection
*/
bool bIsStart(Pulse pulse){
  if(pulse.polarity){
    if(abs(pulse.time - 160) < 19){
      ++ucCountValid;
    }else{
      ucCountValid = 0;
    }
  }else{
    if(abs(pulse.time - 140) < 19){
      ++ucCountValid;
    }else{
      ucCountValid = 0;
    }
  }
  
  if(ucCountValid == 3){
    ucCountValid = 0;
    return TRUE;
  }else{
    return FALSE;
  }
}
/*
*@brief: this function detect stop sequence
*@inval: struct type of pulse
*@retval: bool value of detect
*/
bool bIsStop(Pulse pulse){
  bool bTempVal = FALSE;
  if(!pulse.polarity){
    if(pulse.time > 600){
      bTempVal = TRUE;
    }
  }
  return bTempVal;
}
/*
*@brief: this function detect PWM sequence
*@inval: struct type of pulse
*@retval: bool value of detect
*/
bool bIsPWM(Pulse pulse){
  bool bIsPWM = FALSE;
  if(pulse.time < 25){
    bIsPWM = TRUE;
  }
  return bIsPWM;
}
/*
*@brief: this function combined all of parser into the program
*@inval: current sample(struct Pulse)
*@retval: enumerate state of current sample
*/
State eGetParse(Pulse pulse){
  State eTempState = mistake;
  bool bStart = bIsStart(pulse);
  bool bStop = bIsStop(pulse);
  bool bPWM = bIsPWM(pulse);
  PWMM ePWM = ePWM_Measure(pulse, &ucPWM_Measure);
  if(!bStart&&!bStop){//This is a PWM
    eTempState = mistake;
  }
  if(bStart&!bStop){
    eTempState = start;
  }
  if(bStop){
    eTempState = stop;
  }
  if(bPWM){
    eTempState = pwm;
  }
  
  return eTempState;
}
