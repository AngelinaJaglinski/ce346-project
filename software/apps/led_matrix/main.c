// LED Matrix app
//
// Display messages on the LED matrix

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"

#include "led_matrix.h"
#include "microbit_v2.h"

int la1[8];
int la2[8];

int main(void) {
  printf("Board started!\n");
  
  la1[0] = 72;
  la1[1] = 105;
  la1[2] = 67;
  la1[3] = 69;
  la1[4] = 51;
  la1[5] = 52;
  la1[6] = 54;
  la1[7] = 33;
  la2[0] = 73;
  la2[1] = 116;
  la2[2] = 119;
  la2[3] = 111;
  la2[4] = 114;
  la2[5] = 107;
  la2[6] = 115;
  la2[7] = 33;

  // initialize LED matrix driver
  led_matrix_init(la1, la2);

  // call other functions here

  // loop forever
  while (1) {
    nrf_delay_ms(1000);
  }
}
