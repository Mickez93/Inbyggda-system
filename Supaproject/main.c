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
int main(void) {
int i = 0;
// Enable peripheral clock for GPIOB
RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
// General purpose output mode for LED, PB12 (pin25)
GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE12)) | (GPIO_MODER_MODE12_0);
while(1){
GPIOB->BSRR = GPIO_BSRR_BR_12;
printf("LED now off\n");
i++;
GPIOB->BSRR = GPIO_BSRR_BS_12;
printf("LED now on\n");
}
}

/*************************** End of file ****************************/
