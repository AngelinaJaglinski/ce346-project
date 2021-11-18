#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  uint32_t GPIO;
  uint32_t _unused_A[320];
  uint32_t OUT;
  uint32_t OUTSET;
  uint32_t OUTCLR;
  uint32_t IN;
  uint32_t DIR;
  uint32_t DIRSET;
  uint32_t DIRCLR;
  uint32_t LATCH;
  uint32_t DETECTMODE;
  uint32_t _unused_B[118];
  uint32_t PIN_CNF[32];
} gpio_reg_t;

volatile gpio_reg_t* GPIO_REG0 = (gpio_reg_t*)(0x50000000);
volatile gpio_reg_t* GPIO_REG1 = (gpio_reg_t*)(0x50000300);

void gpio_print() {
  printf("%p\n",  &(GPIO_REG0->DIR));
  printf("%p\n",  &(GPIO_REG0->DIRSET));
  printf("%p\n",  &(GPIO_REG0->OUTSET));
}
  


// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
  // Implement me
  // Hint: Use proper PIN_CNF instead of DIR


  if (gpio_num < 32) {
    if (dir == 0) {
      GPIO_REG0->PIN_CNF[gpio_num] = 0;
    } else {
      GPIO_REG0->PIN_CNF[gpio_num] = 0x3;
    }
  } else {
     if (dir == 0) {
       GPIO_REG1->PIN_CNF[gpio_num-32] = 0;
     } else {
       GPIO_REG1->PIN_CNF[gpio_num-32] = 0x3;
     }
  }

}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_set(uint8_t gpio_num) {
  // Implement me
  if (gpio_num < 32) {
    GPIO_REG0->OUTSET = 1 << gpio_num;
  } else {
    GPIO_REG1->OUTSET = 1 << (gpio_num-32);
  }
  
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_clear(uint8_t gpio_num) {
  // Implement me
  if (gpio_num < 32) {
    GPIO_REG0->OUTCLR = 1 << gpio_num;
  } else {
    GPIO_REG1->OUTCLR = 1 << (gpio_num-32);
  }
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
// Output:
//  bool - pin state (true == high)
bool gpio_read(uint8_t gpio_num) {
  // Implement me

   if (gpio_num < 32) {
     return (GPIO_REG0->IN >> gpio_num) & 1;
  } else {
     return (GPIO_REG1->IN >> (gpio_num-32)) & 1;
  }
}
