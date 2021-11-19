#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Initialize the LED matrix display
void led_matrix_init(void);

// You may need to add more functions here
void disp_char(char write_char);

void disp_str(const char* str);
