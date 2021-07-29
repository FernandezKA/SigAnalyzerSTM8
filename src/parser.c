#include "parser.h"
Pulse xNewSample = {0, FALSE};
bool bNewSample = TRUE;
uint8_t ucCountValid = 0;
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
