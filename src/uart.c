#include "uart.h"
#define DEBUG
#ifdef DEBUG
void vUart_Config(void){
  CLK->PCKENR1|=CLK_PCKENR1_UART1;//ENABLE CLOCKING
  UART1_Init(115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE);
  UART1_Cmd(ENABLE);
}
//void send(uint8_t data){
//  while((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE) {asm("nop");}
//  UART1->DR = data;
//  return;
//}
//
//void print(uint8_t* data, uint8_t size){
//  for(uint8_t i = 0; i < size; ++i){
//    send(data[i]);
//  }
//}

#endif 