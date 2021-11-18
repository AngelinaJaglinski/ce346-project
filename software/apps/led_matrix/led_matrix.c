// LED Matrix Driver
// Displays characters on the LED matrix

#include <stdbool.h>
#include <stdio.h>

#include "nrf_gpio.h"
#include "app_timer.h"

#include "led_matrix.h"
#include "font.h"
#include "nrf_delay.h"
#include "microbit_v2.h"


APP_TIMER_DEF(the_time_1);
bool led_states[5][5];
int letter_index = 0;
int value = 0;
int letter_array[8];
int lay1[8];
int lay2[8];
int count = 0;
int letter = 0;
uint32_t row_leds[] = {LED_ROW1, LED_ROW2, LED_ROW3, LED_ROW4, LED_ROW5};
uint32_t col_leds[] = {LED_COL1, LED_COL2, LED_COL3, LED_COL4, LED_COL5};

//choose grid

static void led_set(void* _unsused){

  for (int b = 0; b<5;b++){
    led_states[0][b] = font[letter][0] & (1 << b);
    led_states[1][b] = font[letter][1] & (1 << b);
    led_states[2][b] = font[letter][2] & (1 << b);
    led_states[3][b] = font[letter][3] & (1 << b);
    led_states[4][b] = font[letter][4] & (1 << b);

  }

  nrf_gpio_pin_set(row_leds[value]);

  for (int x = 0; x<5;x++){
    if (led_states[value][x]){
      nrf_gpio_pin_clear(col_leds[x]);
    }
  }
  nrf_delay_ms(1);
  nrf_gpio_pin_clear(row_leds[value]);
  for (int a = 0; a<5;a++){
    nrf_gpio_pin_set(col_leds[a]);
  }

  if (value == 4){
    value = 0;
  }
  else {
    value = value +1;
  }

  if (count == 500){
    count = 0;
    
    if (letter_index == 7){
      letter_index = 0;
      if (letter_array[letter_index] == lay1[letter_index]){
        memcpy(letter_array, lay2, sizeof(letter_array));
      }
      else {
        memcpy(letter_array, lay1, sizeof(letter_array));
      }
      nrf_delay_ms(1000);
    }
    else {
      letter_index = letter_index +1;
    }
    letter = letter_array[letter_index];
  }
  else {
    count = count +1;

  }
}

void led_matrix_init(int la1[], int la2[]) {
  // initialize row pins
  nrf_gpio_pin_dir_set(LED_ROW1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW2, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW3, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW4, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_ROW5, NRF_GPIO_PIN_DIR_OUTPUT);
  
  nrf_gpio_pin_dir_set(LED_COL1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL2, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL3, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL4, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(LED_COL5, NRF_GPIO_PIN_DIR_OUTPUT);

  nrf_gpio_pin_set(LED_COL1);
  nrf_gpio_pin_set(LED_COL2);
  nrf_gpio_pin_set(LED_COL3);
  nrf_gpio_pin_set(LED_COL4);
  nrf_gpio_pin_set(LED_COL5);
  // set default values for pins
  nrf_gpio_pin_clear(LED_ROW1);
  nrf_gpio_pin_clear(LED_ROW2);
  nrf_gpio_pin_clear(LED_ROW3);
  nrf_gpio_pin_clear(LED_ROW4);
  nrf_gpio_pin_clear(LED_ROW5);
  
  
  memcpy(lay1, la1, sizeof(lay1));
  memcpy(lay2, la2, sizeof(lay2));
  memcpy(letter_array, lay1, sizeof(letter_array));
  letter = letter_array[0];


 // nrf_gpio_pin_set(LED_ROW1);
 // nrf_gpio_pin_set(LED_COL5);
  // initialize timer(s) (Part 3 and onwards)
  app_timer_init();
  app_timer_create(&the_time_1, APP_TIMER_MODE_REPEATED, led_set);
  app_timer_start(the_time_1, 32768/1000, NULL);

  // set default state for the LED display (Part 4 and onwards)
  while(1){
    nrf_delay_ms(1000);
	  
  }
}




