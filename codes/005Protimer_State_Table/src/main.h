#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <stdint.h>
#include "button.h"

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
    MAX_SIGNALS,
}protimer_signals_t;

/*Various States of the Application*/
typedef enum{
    IDLE,
    TIME_SET,
    COUNTDOWN,
    PAUSE,
    STAT,
    MAX_STATES,
}protimer_states_t;
/************************************************/
/*Main application structure*/
typedef struct {
    uint32_t curr_time;
    uint32_t elapsed_time;
    uint32_t pro_time;
    protimer_states_t active_state;
    uintptr_t *state_table;
}protimer_t;
/************************************************/
//Generic (super) event structure*/
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

/*function pointer type for event handlers */
typedef event_status_t (*e_handler_t)(protimer_t* const mobj, event_t const * const e);

/************************************************/
void do_beep(void);
/************************************************/
void protimer_init(protimer_t*mobj);
void protimer_state_table_init(protimer_t* const mobj);
void protimer_event_dispatcher(protimer_t* const mobj, event_t const * const e);

/*prototypes of event handlers*/
/**************************************************************************************/
event_status_t IDLE_ENTRY(protimer_t* const mobj, event_t const * const e);
event_status_t IDLE_EXIT(protimer_t* const mobj, event_t const * const e);
event_status_t IDLE_INC_TIME(protimer_t* const mobj, event_t const * const e);
event_status_t IDLE_START_PAUSE(protimer_t* const mobj, event_t const * const e);
event_status_t IDLE_TIME_TICK(protimer_t* const mobj, event_t const * const e);
/**************************************************************************************/
event_status_t COUNTDOWN_EXIT(protimer_t* const mobj, event_t const * const e);
event_status_t COUNTDOWN_TIME_TICK(protimer_t* const mobj, event_t const * const e);
event_status_t COUNTDOWN_START_PAUSE(protimer_t* const mobj, event_t const * const e);
event_status_t COUNTDOWN_ABRT(protimer_t* const mobj, event_t const * const e);
/**************************************************************************************/
event_status_t STAT_ENTRY(protimer_t* const mobj, event_t const * const e);
event_status_t STAT_EXIT(protimer_t* const mobj, event_t const * const e);
event_status_t STAT_TIME_TICK(protimer_t* const mobj, event_t const * const e);
/**************************************************************************************/
event_status_t TIME_SET_ENTRY(protimer_t* const mobj, event_t const * const e);
event_status_t TIME_SET_EXIT(protimer_t* const mobj, event_t const * const e);
event_status_t TIME_SET_INC_TIME(protimer_t* const mobj, event_t const * const e);
event_status_t TIME_SET_DEC_TIME(protimer_t* const mobj, event_t const * const e);
event_status_t TIME_SET_ABRT(protimer_t* const mobj, event_t const * const e);
event_status_t TIME_SET_START_PAUSE(protimer_t* const mobj, event_t const * const e);
/**************************************************************************************/
event_status_t PAUSE_ENTRY(protimer_t* const mobj, event_t const * const e);
event_status_t PAUSE_EXIT(protimer_t* const mobj, event_t const * const e);
event_status_t PAUSE_INC_TIME(protimer_t* const mobj, event_t const * const e);
event_status_t PAUSE_DEC_TIME(protimer_t* const mobj, event_t const * const e);
event_status_t PAUSE_START_PAUSE(protimer_t* const mobj, event_t const * const e);
event_status_t PAUSE_ABRT(protimer_t* const mobj, event_t const * const e);
/**************************************************************************************/



#endif

