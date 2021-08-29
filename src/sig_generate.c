#include "sig_generate.h"
Pulse sigGen[GEN_SIZE];
uint8_t ucCurrentIndexGen;
uint16_t usCurrentIndexSample;
Pulse sigGen[GEN_SIZE] = {{100, TRUE}, {20, FALSE}, {180, TRUE}, {94, FALSE}, {100, TRUE}, {10, FALSE}, {190, TRUE}, {94, FALSE}};
void vIncrementGenIndex(uint8_t* IndexGen){
  if(*IndexGen == GEN_SIZE - 1){
    *IndexGen = 0;
  }
  else{
    ++*IndexGen;
  }
}
