#include "stm32f4xx.h"

// Using the user button (B1) as an input

#define GPIOCEN (1U << 2)
#define GPIOAEN (1U << 0)
#define PIN13 (1U << 13)
#define PIN5 (1U << 5)

#define BTN_PIN PIN13
#define LED_PIN PIN5

int main(void) {
  // when we press the button, turn the LED on
  RCC->AHB1ENR |= GPIOAEN;
  RCC->AHB1ENR |= GPIOCEN;

  // configure pin5 registers for output (1:0)
  GPIOA->MODER |= (1U << 10);  // set idx 10 to 1
  GPIOA->MODER &= ~(1U << 11); // set idx 11 to 0

  // configure pin13 registers for input (0:0)
  GPIOC->MODER &= ~(1U << 26);
  GPIOC->MODER &= ~(1U << 27);

  while (1) {
    // Check if btn pressed
    if (GPIOC->IDR & BTN_PIN) {
      GPIOA->BSRR = LED_PIN;
    } else {
      GPIOA->BSRR = (1U << 21);
    }
  }
}
