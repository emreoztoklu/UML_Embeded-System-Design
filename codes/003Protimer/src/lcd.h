#ifndef LCD_H
#define LCD_H

#include <main.h>

/*LCD PIN DEFINITION*/
#define PIN_LCD_RS  5
#define PIN_LCD_RW  6
#define PIN_LCD_EN  7
#define PIN_LCD_D4  8
#define PIN_LCD_D5  9
#define PIN_LCD_D6  10
#define PIN_LCD_D7  11
/*********************************************/


void lcd_print_char(char c);
void lcd_scroll_left(void);
void lcd_scroll_right(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_no_auto_scroll(void);

void lcd_move_cursor_R2L(void);
void lcd_move_cursor_L2R(void);
void lcd_cursor_off(void);
void lcd_cursor_blinkoff(void);
void lcd_insert_number(int val);
void lcd_print_string(String string);


void display_init(void);
void display_time(uint32_t time);
void display_message(String msg, uint8_t col, uint8_t row);
void display_clear(void);



#endif
