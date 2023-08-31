

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

void uart_tx(uint8_t txdata){
  while(!(USART1->ISR & USART_ISR_TXE)); // wait
    USART1->TDR = txdata; // send
}
void delay_systicks(uint32_t ticks);

uint8_t uart_rx(void){
while(!(USART1->ISR & USART_ISR_RXNE)); // wait for data
return (uint8_t)(USART1->RDR); // read
}

// Busy-wait delay using SysTick timer.
// Based on Yiu-book, ch. 9.5 "The SysTick Timer"


int main(void) {
  //ENABLA KLOCKOR
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  RCC ->IOPENR |= RCC_IOPENR_GPIOBEN;
  //NOLLA PINNE 6 och 7 i AFR för alternate mode 1
  GPIOB->AFR[0] = (GPIOB->AFR[0] & ~(0xF0000000));
  GPIOB->AFR[0] = (GPIOB->AFR[0] & ~(0xF000000));
  //SÄTT ALTERNATE FUNCTION PÅ PINNE 42/43 PB6/PB7
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE6)) | (GPIO_MODER_MODE6_1);
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE7)) | (GPIO_MODER_MODE7_1);
  //SÄTT KLOCKA
  USART1->BRR = SystemCoreClock / 9600;
  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
  
  int UARTchar;
  while(1)
  { 
    
    uart_tx(0x01);
    printf("test");
    delay_systicks(SystemCoreClock);
    UARTchar = uart_rx();
    printf("%c",UARTchar);
    delay_systicks(SystemCoreClock);

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
