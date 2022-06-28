#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <stdint.h>
#include "button.h"

#define PUBLIC
#define PRIVATE static

#define PIN_BUZZER  12

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

/*Event Handle CASES*/
typedef enum {
    EVENT_HANDLED,
    EVENT_IGNORED,
    EVENT_TRANSITION,
}event_status_t;
/************************************************/
/*EVENTS*/
// Generic (super) event structure*/

typedef struct event_tag{
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
/************************************************/

//forward decleration
struct protimer_tag; 
struct event_tag;

/*fonc pointer for event handler*/
typedef event_status_t(*protimer_state_t)(struct protimer_tag * const, struct event_tag const*const );

/************************************************/
/*Main application structure*/
typedef struct protimer_tag {
    uint32_t curr_time;
    uint32_t elapsed_time;
    uint32_t pro_time;
    protimer_state_t  active_state;
}protimer_t;

/************************************************/
void protimer_init(protimer_t*mobj);
void protimer_event_dispatcher(protimer_t* const mobj, event_t const * const e);
void do_beep(void);




#endif

