#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Initialize the LED matrix display
void led_matrix_init(int la1[], int la2[]);

static void led_set(void* _unused);
// You may need to add more functions here

