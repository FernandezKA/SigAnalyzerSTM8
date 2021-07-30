#include "sig_generate.h"
Pulse sigGen[GEN_SIZE];
uint8_t ucCurrentIndexGen;
uint16_t usCurrentIndexSample;
Pulse sigGen[GEN_SIZE] = {{10, TRUE}, {20, FALSE}, {30, TRUE}, {40, FALSE}, {50, TRUE}, {60, FALSE}};
void vIncrementGenIndex(uint8_t* IndexGen){
  if(*IndexGen == GEN_SIZE - 1){
    *IndexGen = 0;
  }
  else{
    ++*IndexGen;
  }
}
