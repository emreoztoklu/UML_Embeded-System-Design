

#include "lcd.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(PIN_LCD_RS,PIN_LCD_RW,PIN_LCD_EN,PIN_LCD_D4,PIN_LCD_D5,PIN_LCD_D6,PIN_LCD_D7);

void lcd_clear(void){
    lcd.clear();
}
void lcd_print_char(char c){
    lcd.print(c);
}

void lcd_scroll_left(void){
    lcd.scrollDisplayLeft();
}
void lcd_scroll_right(void){
    lcd.scrollDisplayRight();
}
void lcd_set_cursor(int c, int r){
    lcd.setCursor(c,r);
}
void lcd_no_auto_scroll(void){
    lcd.noAutoscroll();
}
void lcd_begin(uint8_t cols , uint8_t rows){
    lcd.begin(cols,rows);
}
void lcd_move_cursor_R2L(void){
    lcd.rightToLeft();
}
void lcd_move_cursor_L2R(void){
    lcd.leftToRight();
}
void lcd_cursor_off(void){
    lcd.noCursor();
}
void lcd_cursor_blinkoff(void){
    lcd.noBlink();
}
void lcd_print_number(int num){
    lcd.print(num);
}
void lcd_print_string(String s){
    lcd.print(s);
}

void lcd_cursor_show(void)
{
    lcd.cursor();
}

void lcd_cursor_blink(void)
{
    lcd.blink();
}

void lcd_create_custom_char(byte num, byte* data) {
    lcd.createChar(num, data);
}

void lcd_display_custom_char(byte num){
    lcd.write(num);
}

void display_init(void){
    lcd_begin(16,2);
    lcd_clear();
    lcd_move_cursor_L2R();
    lcd_set_cursor(0,0);
    lcd_no_auto_scroll();
    lcd_cursor_off();
}

void display_time(uint32_t time){
    char buf[7];
    String time_msg;

    uint16_t min = time / 60;
    uint8_t  sec = time % 60;
    sprintf(buf, "%03d:%02d", min, sec);
    time_msg = (String)buf;
    lcd_set_cursor(0,6);
    lcd_print_string(time_msg);
}

void display_message(String msg, uint8_t col, uint8_t row){
    lcd_set_cursor(row,col);
    lcd_print_string(msg);
}

void display_clear(void){
    lcd_clear();
}


void display_cursor_on_blinkon(void){
    lcd_cursor_show();
    lcd_cursor_blink();
}

void display_cursor_off_blinkoff(void){
    lcd_cursor_off();
    lcd_cursor_blinkoff();
}

void display_set_cursor(uint8_t r, uint8_t c){
     lcd_set_cursor(c,r);
}

void display_erase_block(uint8_t row,uint8_t col_start,uint8_t col_stop)
{
    uint8_t len = col_stop - col_start;
    do{
        lcd_set_cursor(col_start++,row);
        lcd_print_char(' ');
    }while(len--);
}