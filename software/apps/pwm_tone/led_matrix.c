// LED Matrix Driver
// Displays characters on the LED matrix

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "nrf_gpio.h"
#include "app_timer.h"

#include "nrf_delay.h"

#include "led_matrix.h"
#include "font.h"
#include "microbit_v2.h"

static uint32_t row_leds[] = {LED_ROW1, LED_ROW2, LED_ROW3, LED_ROW4, LED_ROW5};
static uint32_t col_leds[] = {LED_COL1, LED_COL2, LED_COL3, LED_COL4 ,LED_COL5};

static bool led_matrix[5][5] = {
  {0,1,1,1,0},
  {1,0,1,0,1},
  {1,1,0,1,1},
  {1,0,1,0,1},
  {0,1,1,1,0},	
};

static bool** bitmap;

APP_TIMER_DEF(my_timer_1);

uint8_t curr_row = 0;

static void display_leds(void* _unused) {

  // Turn off last row
  nrf_gpio_pin_clear(row_leds[curr_row]);
  for (size_t i=0; i < 5; i++){
    nrf_gpio_pin_set(col_leds[i]);
  }

  curr_row = curr_row < 4 ? curr_row +1 : 0;

  nrf_gpio_pin_set(row_leds[curr_row]);
  for (size_t i=0; i < 5; i++){
     nrf_gpio_pin_write(col_leds[i], led_matrix[curr_row][i]);
  }
}

void disp_char(char write_char){
	memset(led_matrix, 0, 25);

	for(size_t i=0; i < 5; i++){
		for(size_t j=0; j < 5; j++){
			led_matrix[i][j] = !(font[(uint8_t) write_char][i] & (1<<j));
		}
	}
	
	app_timer_start(my_timer_1, 65, NULL);
}


static void disp_screen(uint32_t col){
	for(size_t row=0; row < 5; row++){
		for(size_t c_i=col; c_i < col+5; c_i++){
			led_matrix[row][c_i-col] = bitmap[row][c_i];
		}
	}
}

static void init_bitmap(const char* str){ // Updates global bitmap
	bool** new_matrix = malloc(strlen(str)*25); // Bitmap[5][str_length*5]

	uint32_t curr_char = 0;
	while(str[curr_char]) {
		for(size_t row=0; row < 5; row++){	
			for(size_t col=(curr_char*5); col < (curr_char*5)+5; col++){
				new_matrix[row][col] = !(font[(uint8_t)str[curr_char]][row] & (1<<(col - (curr_char*5))));
			}
		}
		curr_char++;	
	}
	bitmap = new_matrix;

	return;
}

void disp_str(const char* str){
	free(bitmap);
	init_bitmap(str);

	for(uint32_t col = 0; col < strlen(str)*5; col++){
		disp_screen(col);
		nrf_delay_ms(100);
	}
	
}

void led_matrix_init(void) {
  // initialize row pins
  for (size_t i=0; i < 5; i++){
  	nrf_gpio_pin_dir_set(row_leds[i], NRF_GPIO_PIN_DIR_OUTPUT);
  }


  // initialize col pins
  for (size_t i=0; i < 5; i++){
  	nrf_gpio_pin_dir_set(col_leds[i], NRF_GPIO_PIN_DIR_OUTPUT);
  }


  // set default values for pins
  for (size_t i=0; i < 5; i++){
    nrf_gpio_pin_clear(row_leds[i]);
  	nrf_gpio_pin_set(col_leds[i]);
  }
  
  // initialize timer(s) (Part 3 and onwards)
  app_timer_init();
  app_timer_create(&my_timer_1, APP_TIMER_MODE_REPEATED, display_leds);
  app_timer_start(my_timer_1, 65, NULL);

  // set default state for the LED display (Part 4 and onwards)
}


