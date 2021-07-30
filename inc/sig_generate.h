#ifndef _sig_generate_h_
#define _sig_generate_h_
#include "general.h"
#define GEN_SIZE 6
extern Pulse sigGen[GEN_SIZE];
extern uint8_t ucCurrentIndexGen;
extern uint16_t usCurrentIndexSample;
void vIncrementGenIndex(uint8_t* IndexGen);
#endif