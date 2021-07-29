#include "parser.h"
Pulse xNewSample = {0, FALSE};
bool bNewSample = TRUE;
uint8_t ucCountValid = 0;
/*
*@brief: this function detect start sequence
*@inval: struct type Pulse
*@retval: bool value of detection
*/
bool bIsStart(Pulse pulse){
  if(pulse.polarity){
    if(abs(pulse.time - 160) < 16){
      ++ucCountValid;
    }else{
      ucCountValid = 0;
    }
  }else{
    if(abs(pulse.time - 140) < 14){
      ++ucCountValid;
    }else{
      ucCountValid = 0;
    }
  }
  
  if(ucCountValid == 6){
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
    if(abs(pulse.time - 800) < 80){
      bTempVal = TRUE;
    }
  }
  return bTempVal;
}
