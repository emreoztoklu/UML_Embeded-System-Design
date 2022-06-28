#ifndef BUTTON_H
#define BUTTON_H

#include "main.h"

#define DEBOUNCE_TIME 50

/*BUTTON PIN DEFINITION*/
#define PIN_BUTTON1 2
#define PIN_BUTTON2 3
#define PIN_BUTTON3 4

/*********************************************/
/*
BUTTON PAD THRUTH TABLE
INC DEC S/P
B1  B2  B3  VALUE  SIGNAL
0   0   1   1       START_PAUSE
0   1   0   2       DEC_TIME
1   0   0   4       INC_TIME
1   1   0   6       ABRT
other values        xx
*/
#define BTN_PAD_VALUE_SP        1
#define BTN_PAD_VALUE_DEC_TIME  2
#define BTN_PAD_VALUE_INC_TIME  4
#define BTN_PAD_VALUE_ABRT      6


typedef enum{
    NOT_PRESSED,
    BOUNCE,
    PRESSED,
}button_state_t;
/************************************************/

uint8_t process_bouncing_button_pad_value(uint8_t btn_pad_value);
 

#endif
