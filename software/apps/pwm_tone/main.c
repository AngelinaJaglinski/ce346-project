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

uint16_t NOTES_ARRAY[] = {261.6256, 261.6256, 391.9954, 391.9954, 440, 440, 391.9954};
int  BUTTONS_ARRAY[] = {8, 8, 15, 15, 17, 17, 15};
uint16_t ASCII_ARRAY[] = {67, 67, 71, 71, 65, 65, 71};
char CHAR_ARRAY[] = {'C', 'C', 'G', 'G', 'A', 'A', 'G'};
bool key_pressed[32];

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

  nrf_gpio_pin_write(OUTPUT0, 1);
  nrf_gpio_pin_write(OUTPUT1, 1);
  nrf_gpio_pin_write(OUTPUT2, 1);
  nrf_gpio_pin_write(OUTPUT3, 1);
  nrf_gpio_pin_write(OUTPUT4, 1);
  nrf_gpio_pin_write(OUTPUT5, 1);
  nrf_gpio_pin_write(OUTPUT6, 1);
  nrf_gpio_pin_write(OUTPUT7, 1);
}


int key_helper() {
  if (nrf_gpio_pin_read(INPUT0) == 1) {
    if (nrf_gpio_pin_read(OUTPUT0) == 1) {
      return 0;
    }
    else if (nrf_gpio_pin_read(OUTPUT1) == 1) {
      return 1;
    }
    else if (nrf_gpio_pin_read(OUTPUT2) == 1) {
      return 2;
    }
    else if (nrf_gpio_pin_read(OUTPUT3) == 1) {
      return 3;
    }
    else if (nrf_gpio_pin_read(OUTPUT4) == 1) {
      return 4;
    }
    else if (nrf_gpio_pin_read(OUTPUT5) == 1) {
      return 5;
    }
    else if (nrf_gpio_pin_read(OUTPUT6) == 1) {
      return 6;
    }
    else if (nrf_gpio_pin_read(OUTPUT7) == 1) {
      return 7;
    }
  }

  else if (nrf_gpio_pin_read(INPUT1) == 1) {
    if (nrf_gpio_pin_read(OUTPUT0) == 1) {
      return 8;
    }
    else if (nrf_gpio_pin_read(OUTPUT1) == 1) {
      return 9;
    }
    else if (nrf_gpio_pin_read(OUTPUT2) == 1) {
      return 10;
    }
    else if (nrf_gpio_pin_read(OUTPUT3) == 1) {
      return 11;
    }
    else if (nrf_gpio_pin_read(OUTPUT4) == 1) {
      return 12;
    }
    else if (nrf_gpio_pin_read(OUTPUT5) == 1) {
      return 13;
    }
    else if (nrf_gpio_pin_read(OUTPUT6) == 1) {
      return 14;
    }
    else if (nrf_gpio_pin_read(OUTPUT7) == 1) {
      return 15;
    }
  }

  else if (nrf_gpio_pin_read(INPUT2) == 1) {
    if (nrf_gpio_pin_read(OUTPUT0) == 1) {
      return 16;
    }
    else if (nrf_gpio_pin_read(OUTPUT1) == 1) {
      return 17;
    }
    else if (nrf_gpio_pin_read(OUTPUT2) == 1) {
      return 18;
    }
    else if (nrf_gpio_pin_read(OUTPUT3) == 1) {
      return 19;
    }
    else if (nrf_gpio_pin_read(OUTPUT4) == 1) {
      return 20;
    }
    else if (nrf_gpio_pin_read(OUTPUT5) == 1) {
      return 21;
    }
    else if (nrf_gpio_pin_read(OUTPUT6) == 1) {
      return 22;
    }
    else if (nrf_gpio_pin_read(OUTPUT7) == 1) {
      return 23;
    }
  }

  else if (nrf_gpio_pin_read(INPUT3) == 1) {
    if (nrf_gpio_pin_read(OUTPUT0) == 1) {
      return 24;

    }
    else if (nrf_gpio_pin_read(OUTPUT1) == 1) {
      return 25;
    }
    else if (nrf_gpio_pin_read(OUTPUT2) == 1) {
      return 26;
    }
    else if (nrf_gpio_pin_read(OUTPUT3) == 1) {
      return 27;
    }
    else if (nrf_gpio_pin_read(OUTPUT4) == 1) {
      return 28;
    }
    else if (nrf_gpio_pin_read(OUTPUT5) == 1) {
      return 29;
    }
    else if (nrf_gpio_pin_read(OUTPUT6) == 1) {
      return 30;
    }
    else if (nrf_gpio_pin_read(OUTPUT7) == 1) {
      return 31;
    }
  }
  
  return 32;
}

bool bool_helper(int key_int, int button_int){
  if (key_int == 32){
    return false;
  } else {
    if (key_int != button_int){
      return true;
    } else {
      return false;
    }
  }
  
}



  /*

  // C4 (Middle C)
  play_tone(261.6256);
  nrf_delay_ms(1000);

  // D4
  play_tone(293.6648);
  nrf_delay_ms(1000);

  // E4
  play_tone(329.6276);
  nrf_delay_ms(1000);

  //F4
  play_tone(349.2282);
  nrf_delay_ms(1000);

  //G4
  play_tone(391.9954);
  nrf_delay_ms(1000);

  //A4
  play_tone(440);
  nrf_delay_ms(1000);

  //B4
  play_tone(493.8833);
  nrf_delay_ms(1000);
  
  //C5
  play_tone(523.2511);
  nrf_delay_ms(1000);

  */
  
int main(void) {
  printf("Board started!\n");

  // initialize PWM
  pwm_init();

  uint i  = 0;
  bool played_once = false;
  //gpio_config(14, 0);
  //gpio_config(23, 0);
  led_matrix_init();

  init_pins();
  
   
  // nrf_gpio_pin_write(ROW_KEY, 1);
  //nrf_gpio_pin_write(COL_KEY, 1);
  

  //turn on one output pin
  //delay
  //call helper
  //delay optional
  //turn off output pin
  //delay
  //repeat

  

  /*
  while(1){

    while(i<sizeof(NOTES_ARRAY)/2){
      

   

      
      
      disp_char(CHAR_ARRAY[i]);
      // instead of !gpio_read(23), we compare key_helper() to buttons_array at i
      if (!played_once | bool_helper(key_helper(), BUTTONS_ARRAY[i])) {
	
        play_tone(NOTES_ARRAY[i]);
	nrf_delay_ms(1000);
	nrfx_pwm_stop(&PWM_INST, true);
	played_once = true;
	//if (!gpio_read(23)){
	//  disp_char('X');
	//}
	}
      nrf_delay_ms(100);
      // printf("%d", !gpio_read(14));

      if (key_helper() == BUTTONS_ARRAY[i]){
      	played_once = false;
	i++;
      }

      if (i==sizeof(NOTES_ARRAY)/2){
	nrfx_pwm_stop(&PWM_INST, true);
      }
     

    
  }
  }
  */
 



// set one output high(?)
//play keys and read all inputs
  while(1) {
      nrf_gpio_pin_write(OUTPUT2, 1);
      nrf_delay_ms(100);
      uint32_t input1 = nrf_gpio_pin_read(OUTPUT0);
      uint32_t input2 = nrf_gpio_pin_read(OUTPUT1);
      uint32_t input3 = nrf_gpio_pin_read(OUTPUT2);
      uint32_t input4 = nrf_gpio_pin_read(OUTPUT3);
      printf("%d %d %d %d\n", input1, input2, input3, input4);

      // start with all outputs low, set one to high
      // read all inputs
      // set output low
      // set next output high and repeat

  }




  


  



  


  // Stop all noises
  nrfx_pwm_stop(&PWM_INST, true);
}

