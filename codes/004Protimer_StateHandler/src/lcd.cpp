#include <lcd.h>
#include <LiquidCrystal.h>

LiquidCrystal  lcd(PIN_LCD_RS, PIN_LCD_RW, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

static void lcd_clear(void){
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

void lcd_set_cursor(uint8_t row, uint8_t col){
    lcd.setCursor(col,row);
}

void lcd_no_auto_scroll(void){
    lcd.noAutoscroll();
}

static void lcd_begin(uint8_t row, uint8_t col){
    lcd.begin(col,row);
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

void lcd_insert_number(int val){
    lcd.print(val);
}

void lcd_print_string(String string){
    lcd.print(string);
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

