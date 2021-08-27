#include "sig_generate.h"
Pulse sigGen[GEN_SIZE];
uint8_t ucCurrentIndexGen;
uint16_t usCurrentIndexSample;
Pulse sigGen[GEN_SIZE] = {{100, FALSE}, {20, TRUE}, {180, FALSE}, {94, TRUE}, {100, FALSE}, {10, TRUE}, {190, FALSE}, {94, TRUE}};
void vIncrementGenIndex(uint8_t* IndexGen){
  if(*IndexGen == GEN_SIZE - 1){
    *IndexGen = 0;
  }
  else{
    ++*IndexGen;
  }
}
