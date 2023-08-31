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
#include "stm32l0xx.h"

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {

  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE8)) | (GPIO_MODER_MODE8_1);
  GPIOA->AFR[1] = (GPIOA->AFR[1] & ~(0xF));
  RCC->CFGR &= (uint32_t) RCC_CFGR_MCOSEL; /* (1) */
  RCC->CFGR |= RCC_CFGR_MCO_SYSCLK | RCC_CFGR_MCO_PRE_1; /* (2) */
   

 while(1)
 {
    printf("test");
 }
}

/*************************** End of file ****************************/
