#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

//lcd connections
#define PIN_LCD_RS  5
#define PIN_LCD_RW  6
#define PIN_LCD_EN  7
#define PIN_LCD_D4  8
#define PIN_LCD_D5  9
#define PIN_LCD_D6  10
#define PIN_LCD_D7  11


void lcd_clear(void);
void lcd_print_char(char c);
void lcd_scroll_left(void);
void lcd_scroll_right(void);
void lcd_set_cursor(int c, int r);
void lcd_no_auto_scroll(void);
void lcd_begin(uint8_t cols, uint8_t rows);
void lcd_move_cursor_R2L(void);
void lcd_move_cursor_L2R(void);
void lcd_cursor_off(void);
void lcd_cursor_blinkoff(void);
void lcd_print_number(int num);
void lcd_print_string(String s);
void lcd_cursor_blink(void);
void lcd_cursor_show(void);
void lcd_create_custom_char(byte num, byte *data);
void lcd_display_custom_char(byte num);


void display_init(void);
void display_time(uint32_t time);
void display_message(String msg, uint8_t col, uint8_t row);
void display_clear(void);

void display_cursor_on_blinkon(void);
void display_cursor_off_blinkoff(void);
void display_set_cursor(uint8_t r, uint8_t c);

void display_erase_block(uint8_t row,uint8_t col_start,uint8_t col_stop);



#endif 


