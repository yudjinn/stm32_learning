// Led info
// Port: A
// Pin: 5

#include "stm32f4xx.h"

#define GPIOAEN                 (1U<<0)
#define PIN5                    (1U<<5)
#define LED_PIN                 PIN5

int main(void)
{
  // Enable clock access to GPIOA
  RCC->AHB1ENR |= GPIOAEN;

  // configure pin5 registers
  GPIOA->MODER |= (1U<<10); // set idx 10 to 1
  GPIOA->MODER &=~ (1U<<11); // set idx 11 to 0
  while(1)
  {
    GPIOA->BSRR = LED_PIN; 
    for(int i=0; i<=100000;i++){}

    // in BSRR, the BR is 16 bits above the set pin (and BS = PA5 pin)
    GPIOA->BSRR = (1U<<21); 
    for(int i=0; i<=100000;i++){}
  }
}
