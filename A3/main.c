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
  int i = 0;
  // Enable peripheral clock for GPIOA and B
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
  // General purpose output mode for PA5 and PA4
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE5)) | (GPIO_MODER_MODE5_0);
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE4)) | (GPIO_MODER_MODE4_0);
  //Input mode for PB3 and PB7
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE3));
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE7));
  //Enable pull up for PB3
  GPIOB->PUPDR = (GPIOB->PUPDR & ~(GPIO_PUPDR_PUPD3)) |(GPIO_PUPDR_PUPD3_0);
  //Enable pull down for PB7
  GPIOB->PUPDR = (GPIOB->PUPDR & ~(GPIO_PUPDR_PUPD7)) |(GPIO_PUPDR_PUPD7_1);
  //
  uint8_t btn_1_flg = 0;
  uint8_t btn_0_flg = 0;
  GPIOA->BSRR = GPIO_BSRR_BS_4;

  while(1){

   

    delay_systicks(0x00100000);
    
    if(GPIOB->IDR == 0)
    {
      if(btn_1_flg)
      {
        GPIOA->BSRR = GPIO_BSRR_BR_5;
        btn_1_flg = 0;
      }
      else
      {
        GPIOA->BSRR = GPIO_BSRR_BS_5;
        btn_1_flg = 1;
      }
    }

    else if ((GPIOB->IDR & GPIO_IDR_ID7) && (GPIOB->IDR & GPIO_IDR_ID3))
    {
      if(btn_0_flg)
      {
        GPIOA->BSRR = GPIO_BSRR_BR_4;
        btn_0_flg = 0;
      }
      else
      {
        GPIOA->BSRR = GPIO_BSRR_BS_4;
        btn_0_flg = 1;
      }
    }

     else if (GPIOB->IDR & GPIO_IDR_ID7)
    {
      if(btn_1_flg)
      {
        GPIOA->BSRR = GPIO_BSRR_BR_4;
        GPIOA->BSRR = GPIO_BSRR_BS_5;
        btn_1_flg = 0;
      }
      else
      {
        GPIOA->BSRR = GPIO_BSRR_BS_4;
        GPIOA->BSRR = GPIO_BSRR_BR_5;
        btn_1_flg = 1;
      }
    }

    else
    {
     GPIOA->BSRR = GPIO_BSRR_BS_4;
     GPIOA->BSRR = GPIO_BSRR_BR_5;
    }

    

    printf(" Input = %d\n ", GPIOB->IDR);

    delay_systicks(0x00100000);
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
