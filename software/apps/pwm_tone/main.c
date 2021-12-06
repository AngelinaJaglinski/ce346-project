// PWM Tone App
//
// Use PWM to play a tone over the speaker

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"
#include "gpio.h"
#include "app_timer.h"
#include "led_matrix.h"


#define OUTPUT0 EDGE_P0
#define OUTPUT1 EDGE_P1
#define OUTPUT2 EDGE_P2
#define OUTPUT3 EDGE_P5
#define OUTPUT4 EDGE_P8
#define OUTPUT5 EDGE_P9
#define OUTPUT6 EDGE_P11
#define OUTPUT7 EDGE_P12

#define INPUT0 EDGE_P13
#define INPUT1 EDGE_P14
#define INPUT2 EDGE_P15
#define INPUT3 EDGE_P16

APP_TIMER_DEF(APP_TIMER);

// PWM configuration
static const nrfx_pwm_t PWM_INST = NRFX_PWM_INSTANCE(0);

// Holds duty cycle values to trigger PWM toggle
nrf_pwm_values_common_t sequence_data[1] = {0};

// Sequence structure for configuring DMA
nrf_pwm_sequence_t pwm_sequence = {
  .values.p_common = sequence_data,
  .length = 1,
  .repeats = 0,
  .end_delay = 0,
};


static void pwm_init(void) {
  // Initialize the PWM
  // SPEAKER_OUT is the output pin, mark the others as NRFX_PWM_PIN_NOT_USED
  // Set the clock to 500 kHz, count mode to Up, and load mode to Common
  // The Countertop value doesn't matter for now. We'll set it in play_tone()
  nrfx_pwm_config_t pwm_config = {
    .output_pins = {SPEAKER_OUT, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED},
    .base_clock = NRF_PWM_CLK_500kHz,
    .count_mode = NRF_PWM_MODE_UP,
    .load_mode = NRF_PWM_LOAD_COMMON,
    .step_mode = NRF_PWM_STEP_AUTO
  };
  nrfx_pwm_init(&PWM_INST, &pwm_config, NULL);
}

static void play_tone(uint16_t frequency) {
  // Stop the PWM (and wait until its finished)
  nrfx_pwm_stop(&PWM_INST, true);

  // Set a countertop value based on desired tone frequency
  // You can access it as NRF_PWM0->COUNTERTOP
  NRF_PWM0->COUNTERTOP = 500000 / frequency;

  // Modify the sequence data to be a 25% duty cycle
  sequence_data[0] = (500000 / 2) / frequency;

  // Start playback of the samples and loop indefinitely
  nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);
}

//int song_index = 1;

//When choosing song, get rid of number in array names for the one we are playing and add number to song we are replacing.
//Twinkle Twinkle
uint16_t NOTES_ARRAY1[] = {261.6256, 261.6256, 391.9954, 391.9954, 440, 440,
			  391.9954, 349.2282, 349.2282, 329.6276, 329.6276,
			  293.6648, 293.6648, 261.6256, 391.9954, 391.9954,
			  349.2282, 349.2282, 329.6276, 329.6276, 293.6648,
			  391.9954, 391.9954, 349.2282, 349.2282, 329.6276,
			  329.6276, 293.6648, 261.6256, 261.6256, 391.9954,
			  391.9954, 440, 440, 391.9954, 349.2282, 349.2282,
			  329.6276, 329.6276, 293.6648, 293.6648, 261.6256};
int  BUTTONS_ARRAY1[] = {7, 7, 14, 14, 16, 16, 14, 12, 12, 11, 11, 9, 9, 7,
			14, 14, 12, 12, 11, 11, 9, 14, 14, 12, 12, 11, 11,
			9, 7, 7, 14, 14, 16, 16, 14, 12, 12, 11, 11, 9,
			9, 7};
char CHAR_ARRAY1[] = {'C', 'C', 'G', 'G', 'A', 'A', 'G', 'F', 'F', 'E', 'E',
		    'D', 'D', 'C', 'G', 'G', 'F', 'F', 'E', 'E', 'D', 'G',
		    'G', 'F', 'F', 'E', 'E', 'D', 'C', 'C', 'G', 'G', 'A',
		    'A', 'G', 'F', 'F', 'E', 'E', 'D', 'D', 'C'};

//Mary had a Little Lamb
uint16_t NOTES_ARRAY[] = {329.6276, 293.6648, 261.6256, 293.6648, 329.6276,
		       329.6276, 329.6276, 293.6648, 293.6648, 293.6648,
		       329.6276, 391.9954, 391.9954, 329.6276, 293.6648,
		       261.6256, 293.6648, 329.6276, 329.6276, 329.6276,
		       329.6276, 293.6648, 293.6648, 329.6276, 293.6648,
		       261.6276};
int BUTTONS_ARRAY[] = {11, 9, 7, 9, 11, 11, 11, 9, 9, 9, 11, 14, 14, 11,
			9, 7, 9, 11, 11, 11, 11, 9, 9, 11, 9, 7};
char CHAR_ARRAY[] = {'E', 'D', 'C', 'D', 'E', 'E', 'E', 'D', 'D', 'D', 'E',
		      'G', 'G', 'E', 'D', 'C', 'D', 'E', 'E', 'E', 'E', 'D',
		      'D', 'E', 'D', 'C'};

// We Wish You a Merry Christmas
uint16_t NOTES_ARRAY3[] = {391.9954, 523.2511, 523.2511, 587.3295, 523.2511,
			  493.8833, 440, 440, 440, 587.3295, 587.3295, 659.2551,
			  587.3295, 523.2511, 493.8833, 391.9954, 391.9954,
			  659.2551, 659.2551, 698.4565, 659.2551, 587.3295,
			  523.2511, 440, 391.9954, 391.9954, 440, 587.3295,
			  493.8833, 554.3653, 391.9954, 523.2511, 523.2511,
			  523.2511, 493.8833, 493.8833, 523.2511, 493.8833,
			  440, 391.9954, 587.3295, 659.2551, 587.3295, 523.2511,
			  783.9909, 391.9954, 391.9954, 391.9954, 440, 587.3295,
			  493.8833, 523.2511};
int BUTTONS_ARRAY3[] = {14, 19, 19, 21, 19, 18, 16, 16, 16, 21, 21, 23, 21, 19,
		       18, 14, 14, 23, 23, 24, 23, 21, 19, 16, 14, 14, 16, 21,
		       18, 19, 14, 19, 19, 19, 18, 18, 19, 18, 16, 14, 21, 23,
		       21, 19, 26, 14, 14, 14, 16, 21, 18, 19};
char CHAR_ARRAY3[] = {'G', 'C', 'C', 'D', 'C', 'B', 'A', 'A', 'A', 'D', 'D', 'E', 
		      'D', 'C', 'B', 'G', 'G', 'E', 'E', 'F', 'E', 'D', 'C', 'A',
		      'G', 'G', 'A', 'D', 'B', 'C', 'G', 'C', 'C', 'C', 'B', 'B',
		      'C', 'B', 'A', 'G', 'D', 'E', 'D', 'C', 'G', 'G', 'G', 'G',
		      'A', 'D', 'B', 'C'};

//Happy Birthday
uint16_t NOTES_ARRAY4[] = {391.9954, 391.9954, 440, 391.9954, 523.2511, 493.8833,
			  391.9954, 391.9954, 440, 391.9954, 587.3295, 523.2511,
			  391.9954, 391.9954, 783.9909, 659.2551, 523.2511,
			  493.8833, 440, 698.4565, 698.4565, 659.2551, 523.2511,
			  587.3295, 523.2511};
int BUTTONS_ARRAY4[] = {14, 14, 16, 14, 19, 18, 14, 14, 16, 14, 21, 19, 14, 14,
		       26, 23, 19, 18, 16, 24, 24, 23, 19, 21, 19};
char CHAR_ARRAY4[] = {'G', 'G', 'A', 'G', 'C', 'B', 'G', 'G', 'A', 'G', 'D', 'C',
		     'G', 'G', 'G', 'E', 'C', 'B', 'A', 'F', 'F', 'E', 'C', 'D', 'C'};

//Ode to Joy
uint16_t NOTES_ARRAY5[] = {329.6276, 329.6276, 349.2282, 391.9954, 391.9954,
			  349.2282, 329.6276, 293.6648, 261.6256, 261.6256,
			  293.6648, 329.6276, 329.6276, 293.6648, 293.6648,
			  329.6276, 329.6276, 349.2282, 391.9954, 391.9954,
			  349.2282, 329.6276, 293.6648, 261.6256, 261.6256,
			  293.6648, 329.6276, 293.6648, 261.6256, 261.6256,
			  293.6648, 293.6648, 329.6276, 261.6256, 293.6648,
			  329.6276, 349.2282, 329.6276, 261.6256, 293.6648,
			  329.6276, 349.2282, 329.6276, 293.6648, 261.6256,
			  293.6648, 391.9954, 329.6276, 329.6276, 349.2282, 
			  391.9954, 391.9954, 349.2282, 329.6276, 293.6648, 
			  261.6256, 261.6256, 293.6648, 329.6276, 293.6648, 
			  261.6256, 261.6256};
int BUTTONS_ARRAY5[] = {11, 11, 12, 14, 14, 12, 11, 9, 7, 7, 9, 11, 11, 9, 9,
		       11, 11, 12, 14, 14, 12, 11, 9, 7, 7, 9, 11, 9, 7, 7,
		       9, 9, 11, 7, 9, 11, 12, 11, 7, 9, 11, 12, 11, 9, 7, 9, 14,
		       11, 11, 12, 14, 14, 12, 11, 9, 7, 7, 9, 11, 9, 7, 7};
char CHAR_ARRAY5[] = {'E', 'E', 'F', 'G', 'G', 'F', 'E', 'D', 'C', 'C', 'D', 'E',
		     'E', 'D', 'D', 'E', 'E', 'F', 'G', 'G', 'F', 'E', 'D', 'C', 'C',
		     'D', 'E', 'D', 'C', 'C', 'D', 'D', 'E', 'C', 'D', 'E', 'F', 'E', 'C',
		     'D', 'E', 'F', 'E', 'D', 'C', 'D', 'G', 'E', 'E', 'F', 'G', 'G', 'F',
		     'E', 'D', 'C', 'C', 'D', 'E', 'D', 'C', 'C'};

//Jingle Bells
uint16_t NOTES_ARRAY6[] = {329.6276, 329.6276, 329.6276, 329.6276, 329.6276, 329.6276,
			  329.6276, 391.9954, 261.6256, 293.6648, 329.6276,
			  349.2282, 349.2282, 349.2282, 349.2282, 349.2282,
			  329.6276, 329.6276, 329.6276, 329.6276, 329.6276,
			  293.6648, 293.6648, 329.6276, 261.6256, 391.9954,
			  329.6276, 329.6276, 329.6276, 329.6276, 329.6276, 329.6276,
			  329.6276, 391.9954, 261.6256, 293.6648, 329.6276,
			  349.2282, 349.2282, 349.2282, 349.2282, 349.2282,
			  329.6276, 329.6276, 329.6276, 329.6276, 391.9954,
			  391.9954, 349.2282, 293.6648, 261.6256};
int BUTTONS_ARRAY6[] = {11, 11, 11, 11, 11, 11, 11, 14, 7, 9, 11,
		       12, 12, 12, 12, 12, 11, 11, 11, 11, 11, 9, 9, 11, 9, 14,
		       11, 11, 11, 11, 11, 11, 11, 14, 7, 9, 11,
		       12, 12, 12, 12, 12, 11, 11, 11, 11, 14, 14, 12, 9, 7};
char CHAR_ARRAY6[] = {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'G', 'C', 'D', 'E',
		     'F', 'F', 'F', 'F', 'F', 'E', 'E', 'E', 'E', 'E', 'D',
		     'D', 'E', 'D', 'G', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 
		     'G', 'C', 'D', 'E', 'F', 'F', 'F', 'F', 'F', 'E', 'E',
		     'E', 'E', 'G', 'G', 'F', 'D', 'C'};



bool key_pressed[32];
uint32_t outputs[] = {OUTPUT0, OUTPUT1, OUTPUT2, OUTPUT3, OUTPUT4, OUTPUT5, OUTPUT6, OUTPUT7};
bool key_bool[32] = {false};
int i = 0;
int key_num = 32;


/*
while(1){
  read all keys;
  if key is pressed, put true in the key_pressed array;
 }
*/


//initialize led matrix?
int la1[1];
int la2[1];
//map each piano key to a number 0 thru 5
// see what key the user pressed and compare it to BUTTONS_ARRAY[i]
// create function to map buttons to nums 0-5

  /*
while(1):
  microbit plays current note
  delay
  if user plays current note:
      pop current note
  delay
  

play_next_note():
pops first note in notes linked list
  */


void init_pins() {
  nrf_gpio_pin_dir_set(OUTPUT0, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(OUTPUT1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(OUTPUT2, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(OUTPUT3, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(OUTPUT4, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(OUTPUT5, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(OUTPUT6, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(OUTPUT7, NRF_GPIO_PIN_DIR_OUTPUT);

  nrf_gpio_pin_dir_set(INPUT0, NRF_GPIO_PIN_DIR_INPUT);
  nrf_gpio_pin_dir_set(INPUT1, NRF_GPIO_PIN_DIR_INPUT);
  nrf_gpio_pin_dir_set(INPUT2, NRF_GPIO_PIN_DIR_INPUT);
  nrf_gpio_pin_dir_set(INPUT3, NRF_GPIO_PIN_DIR_INPUT);

}

void key_helper(void* _unused) {
  for (int j = 0; j < 8; j++) {
    if (j != i) {
      nrf_gpio_pin_write(outputs[j], 0);	
    }
  }
  
  nrf_gpio_pin_write(outputs[i], 1);
  if (nrf_gpio_pin_read(INPUT0) == 1) {
    key_num = i;
  }
  else if (nrf_gpio_pin_read(INPUT1) == 1) {
    key_num = 8 + i;
  }
  else if (nrf_gpio_pin_read(INPUT2) == 1) {
    key_num = 16 + i;
  }
  else if (nrf_gpio_pin_read(INPUT3) == 1) {
    key_num = 24 + i;
  }
  
  else {
    if (i == 7) {
      key_num = 32;
    }
  }
  i = i+1;
  if (i > 7) {
    i = 0;
  }
  //printf("%d\n", key_num);
}

bool bool_helper(int button_int){
  if (key_num == 32){
    return false;
  } else {
    if (key_num != button_int){
      return true;
    } else {
      return false;
    }
  }
  
}
  
int main(void) {
  printf("Board started!\n");

  // initialize PWM
  pwm_init();

  uint i2  = 0;
  //bool played_once = false;
  led_matrix_init();

  init_pins();
  //app_timer_init();

  app_timer_create(&APP_TIMER, APP_TIMER_MODE_REPEATED, key_helper);
  app_timer_start(APP_TIMER, 32768/1000, NULL);

  //turn on one output pin
  //delay
  //call helper
  //delay optional
  //turn off output pin
  //delay
  //repeat   
  
  while(1){
    while(i2<sizeof(NOTES_ARRAY)/2){
      disp_char(CHAR_ARRAY[i2]);
      if (bool_helper(BUTTONS_ARRAY[i2])) {
        play_tone(55.0);
	nrf_delay_ms(500);
	nrfx_pwm_stop(&PWM_INST, true);
	//played_once = true;
	//if (!gpio_read(23)){
	//  disp_char('X');
	//}
	}
      nrf_delay_ms(20);

      if (key_num == BUTTONS_ARRAY[i2]){
      	//played_once = false;
	play_tone(NOTES_ARRAY[i2]);
	nrf_delay_ms(500);
	nrfx_pwm_stop(&PWM_INST, true);
	i2++;
      }

      if (i2==sizeof(NOTES_ARRAY)/2){
	nrfx_pwm_stop(&PWM_INST, true);
      }
    }
  }

// set one output high(?)
//play keys and read all inputs
  /*
  while(1) {
      nrf_gpio_pin_write(OUTPUT2, 1);
      nrf_delay_ms(100);
      uint32_t input1 = nrf_gpio_pin_read(INPUT0);
      uint32_t input2 = nrf_gpio_pin_read(INPUT1);
      uint32_t input3 = nrf_gpio_pin_read(INPUT2);
      uint32_t input4 = nrf_gpio_pin_read(INPUT3);
      printf("%d %d %d %d\n", input1, input2, input3, input4);

      // start with all outputs low, set one to high
      // read all inputs
      // set output low
      // set next output high and repeat

  }
  */
  // Stop all noises
  nrfx_pwm_stop(&PWM_INST, true);
}

