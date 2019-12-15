/* ----------------------------------------------------------------------------------------
 * LCD write functions
 * Board: Grove - 16x2 LCD (White on Blue) / with HiFive1
 * Device: Seeed Studio	PN 104020111
 * ------------------------------------------------------------------------------------- */
#include <cstring>
#include "LCD_GROVE_16x2.h"
#include "rgb_lcd.h"

rgb_lcd lcd;


/* ----------------------------------------------------------------------------------------
 *  lcd_init: function to initalize the LCD module with pre-set message
 *  returns: void
 * ------------------------------------------------------------------------------------- */
void lcd_init(){
  char line1[] = "Air Quality";  //TODO MOVE THIS OUT OF Library
  char line2[] = "Monitor";      //TODO MOVE THIS OUT OF Library

  // set up the LCD's number of columns and rows:
  lcd.begin(COLUMN_CHARS, COLUMN_CHARS);
  // turn ON display
  lcd.display();
  // clear display
  lcd.clear();

  // Print a message to center LCD line.
  print_align_center(line1, FIRST_ROW);
  print_align_center(line2, SECOND_ROW);

  delay(DELAY_ON_INIT) ;
} // end of lcd_init

/* ----------------------------------------------------------------------------------------
 *  lcd_clear: wrapper function to clear the LCD display
 *  returns: void
 * ------------------------------------------------------------------------------------- */
void lcd_clear(){
  lcd.clear();
} // end of lcd_clear


/* ----------------------------------------------------------------------------------------
 *  print_align_center:  function to display a line on LCD with center alignment
 *  input: line_str - pointer to the string,  row - line number (0 or 1 ; FIRST_ROW/SECOND_ROW)
 *  returns: void
 * ------------------------------------------------------------------------------------- */
void print_align_center(char* line_str, unsigned char row){
  unsigned char cursor_column = 0;

  if ((line_str!=NULL) && (row>=0) && (row<NUM_OF_ROWS))
  {
    // set the cursor to line 0/1 and calculate start column to center text
    cursor_column = (COLUMN_CHARS - strlen(line_str))/2 + 1;
    if (cursor_column<0) cursor_column = 0;

    lcd.setCursor(cursor_column, row);
    // Print a message to the LCD.
    lcd.print(line_str);
  }

} // end of print_align_center
