/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include <stdlib.h>

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
#include <stdio.h>
#include <stdlib.h>
#include "stm32l0xx.h"

void delay_systicks(uint32_t ticks);

int main(void) {
  //ENABLA KLOCKOR
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  RCC ->IOPENR |= RCC_IOPENR_GPIOBEN;
  //SÄTT ALTERNATE FUNCTION PÅ PINNE 42/43 PB6/PB7
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE6)) | (GPIO_MODER_MODE6);
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE7)) | (GPIO_MODER_MODE7);
  
  while(1)
  {
    printf("test");
  }
}

// Busy-wait delay using SysTick timer.
// Based on Yiu-book, ch. 9.5 "The SysTick Timer"
void delay_systicks(uint32_t ticks) {
SysTick->CTRL = 0; // disable systick
SysTick->LOAD = ticks - 1; // count from (ticks-1) to 0
// (i.e. ticks cycles)
SysTick->VAL = 0; // clear current value and
// count flag
SysTick->CTRL = (1 << 2) | (1 << 0); // enable systick timer
// with processor clock
while ((SysTick->CTRL & (1 << 16)) == 0){
// wait until count flag is set
}
SysTick->CTRL = 0; // disable systick
}


/*************************** End of file ****************************/

/*************************** End of file ****************************/