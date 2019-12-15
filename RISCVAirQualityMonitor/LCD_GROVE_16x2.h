#ifndef __LCD_GROVE_16X2__
#define __LCD_GROVE_16X2__

#define COLUMN_CHARS 16
#define NUM_OF_ROWS 2
#define FIRST_ROW 0
#define SECOND_ROW 1
#define DELAY_ON_INIT 500

#include <Arduino.h>
#include "SSWire_AirQualityMonitor.h"

void lcd_init();
void print_align_center(char* line_str, unsigned char row);

#endif
