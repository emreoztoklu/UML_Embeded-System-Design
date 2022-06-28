#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <stdint.h>

#define PUBLIC
#define PRIVATE static


/*BUTTON PIN DEFINITION*/
#define PIN_BUTTON1 2
#define PIN_BUTTON2 3
#define PIN_BUTTON3 4
#define PIN_BUZZER  12
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

/* SIGNALS & STATES*/
/*Signal of the Application*/
typedef enum {
    INC_TIME,
    DEC_TIME,
    TIME_TICK,
    START_PAUSE,
    ABRT,
/*internal Activity signals*/
    ENTRY,
    EXIT,
}protimer_signals_t;

/*Various States of the Application*/
typedef enum{
    IDLE,
    TIME_SET,
    COUNTDOWN,
    PAUSE,
    STAT,
}protimer_states_t;
/************************************************/
/*Main application structure*/
typedef struct {
    uint32_t curr_time;
    uint32_t elapsed_time;
    uint32_t pro_time;
    protimer_states_t active_state;
}protimer_t;
/************************************************/
/*EVENTS*/

/*For user generated evetns*/
/* typedef struct{
    uint8_t sig;
}protimer_user_event_t; */

/*For Tick event*/
/* typedef struct {
    uint8_t sig;
    uint8_t ss;   //subsecond
}protimer_tick_event_t; */

/*OR
  Generic (super) event structure*/

typedef struct{
    uint8_t sig;
}event_t;

/*For user generated events*/
typedef struct{
    event_t super;
}protimer_user_event_t;

/*For tick event*/
typedef struct{
    event_t super;
    uint8_t ss;
}protimer_tick_event_t;

typedef enum{
    EVENT_HANDLED,
    EVENT_IGNORED,
    EVENT_TRANSITION,
}event_status_t;

/************************************************/
typedef enum{
    NOT_PRESSED,
    BOUNCE,
    PRESSED,
}button_state_t;

/************************************************/
void protimer_init(protimer_t*mobj);
event_status_t protimer_state_machine(protimer_t* const mobj, event_t const * const e );
void protimer_event_dispatcher(protimer_t* const mobj, event_t const * const e);
void do_beep(void);




#endif

