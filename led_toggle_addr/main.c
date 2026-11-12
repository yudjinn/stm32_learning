// Led info
// Port: A
// Pin: 5

#include "stm32f4xx.h"

#define GPIOAEN                 (1U<<0)
#define PIN5                    (1U<<5)
#define LED_PIN                 PIN5

// Using raw clock/pin access
int main(void)
{
  // Enable clock access to GPIOA
  RCC->AHB1ENR |= GPIOAEN;

  // configure pin5 registers
  GPIOA->MODER |= (1U<<10); // set idx 10 to 1
  GPIOA->MODER &=~ (1U<<11); // set idx 11 to 0
  while(1)
  {
    GPIOA->ODR ^= LED_PIN; // toggle led pin state 1<->0
    for(int i=0; i<=100000;i++){}
  }
}


