#include "clocking.h"
void vClock_Config(void){
 CLK->CKDIVR = 0x00;                    // Предделитель равен нулю
}