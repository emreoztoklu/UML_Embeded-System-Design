#include "main.h"
#include "lcd.h"

void protimer_init(protimer_t* mobj){
    event_t event_enter_action;
    e_handler_t ehandler;

    ehandler =(e_handler_t)mobj->state_table [IDLE * MAX_SIGNALS + ENTRY];

    mobj->active_state = IDLE;  
    event_enter_action.sig = ENTRY;

    mobj->pro_time = 0;
   
    (*ehandler)(mobj, &event_enter_action);
}

void protimer_state_table_init(protimer_t* const mobj){
    static e_handler_t protimer_state_table[MAX_STATES][MAX_SIGNALS] = {
      [IDLE]     = {&IDLE_INC_TIME, NULL, &IDLE_TIME_TICK, &IDLE_START_PAUSE, NULL, &IDLE_ENTRY, &IDLE_EXIT},
      [TIME_SET] = {&TIME_SET_INC_TIME, &TIME_SET_DEC_TIME, NULL, &TIME_SET_START_PAUSE, &TIME_SET_ABRT, &TIME_SET_ENTRY, &TIME_SET_EXIT},
      [COUNTDOWN]= {NULL, NULL, &COUNTDOWN_TIME_TICK, &COUNTDOWN_START_PAUSE, &COUNTDOWN_ABRT, NULL, &COUNTDOWN_EXIT}, 
      [PAUSE]    = {&PAUSE_INC_TIME, &PAUSE_DEC_TIME, NULL, &PAUSE_START_PAUSE, &PAUSE_ABRT, &PAUSE_ENTRY, &PAUSE_EXIT},
      [STAT]     = {NULL, NULL, &STAT_TIME_TICK, NULL, NULL, &STAT_ENTRY, &STAT_EXIT},    
    };

    mobj->state_table = (uintptr_t*) &protimer_state_table[0][0];
}


void protimer_event_dispatcher(protimer_t* const mobj, event_t const * const e){
    event_status_t status;
    protimer_states_t source, target;     //dummy variable
    e_handler_t ehandler;                 //func pointer
     
    source   = mobj->active_state;
    ehandler = (e_handler_t)mobj->state_table[mobj->active_state * MAX_SIGNALS + e->sig];
    
    if(ehandler)
        status   = (*ehandler)(mobj, e);

    if (status == EVENT_TRANSITION){
        target = mobj->active_state;
        event_t ee;
    //1.run exit action for the source state
        ee.sig = EXIT;
        mobj->active_state = source;
        ehandler = (e_handler_t)mobj->state_table[source * MAX_SIGNALS + EXIT];
        if(ehandler)
            (*ehandler)(mobj, &ee);

    //2. run the entry action for the target state
        ee.sig = ENTRY;
        mobj->active_state = target;
        ehandler = (e_handler_t)mobj->state_table[target * MAX_SIGNALS + ENTRY];
        if(ehandler)
            (*ehandler)(mobj, &ee);
    }
}

event_status_t IDLE_ENTRY(protimer_t* const mobj, event_t const * const e) {
    mobj->curr_time = 0;
    mobj->elapsed_time = 0;
    display_time(0);
    display_message("Set",0,0);
    display_message("Time",0,1);
    return EVENT_HANDLED;
}
event_status_t IDLE_EXIT(protimer_t* const mobj, event_t const * const e) {
    display_clear();
    return EVENT_HANDLED;    
}
event_status_t IDLE_INC_TIME(protimer_t* const mobj, event_t const * const e) {
    mobj->curr_time +=60;
    mobj->active_state = TIME_SET;
    return EVENT_TRANSITION;
}
event_status_t IDLE_START_PAUSE(protimer_t* const mobj, event_t const * const e) {
    mobj->active_state = STAT;
    return EVENT_TRANSITION;
}
event_status_t IDLE_TIME_TICK(protimer_t* const mobj, event_t const * const e) {
    if(((protimer_tick_event_t*)(e))->ss == 5){
        do_beep();
        return EVENT_HANDLED;
    }
    return EVENT_IGNORED;  
}
/**************************************************************************************/
event_status_t COUNTDOWN_EXIT(protimer_t* const mobj, event_t const * const e) {
    mobj->pro_time += mobj->elapsed_time;
    mobj->elapsed_time = 0;
    return EVENT_HANDLED;
}
event_status_t COUNTDOWN_TIME_TICK(protimer_t* const mobj, event_t const * const e) {
    static uint8_t tick_count;
    if(++tick_count == 10){        //((protimer_tick_event_t*)(e))->ss == 10
        tick_count = 0;
        --mobj->curr_time;
        ++mobj->elapsed_time;
        display_time(mobj->curr_time);
        if(!mobj->curr_time){
            mobj->active_state = IDLE;
            return EVENT_TRANSITION;
        }
        return EVENT_HANDLED;
    }
    return EVENT_IGNORED;
}
event_status_t COUNTDOWN_START_PAUSE(protimer_t* const mobj, event_t const * const e) {
    mobj->active_state = PAUSE;
    return EVENT_TRANSITION;
}
event_status_t COUNTDOWN_ABRT(protimer_t* const mobj, event_t const * const e) {
    mobj->active_state = IDLE;
    return EVENT_TRANSITION;
}
/**************************************************************************************/
event_status_t STAT_ENTRY(protimer_t* const mobj, event_t const * const e) {
    display_time(mobj->pro_time);
    display_message("Productive Time",1,1);
    return EVENT_HANDLED;
}
event_status_t STAT_EXIT(protimer_t* const mobj, event_t const * const e) {
    display_clear();
    return EVENT_HANDLED;
}
event_status_t STAT_TIME_TICK(protimer_t* const mobj, event_t const * const e) {
     static uint8_t tick_count;
     if(++tick_count == 30){
                tick_count = 0;
                mobj->active_state = IDLE;
                return EVENT_TRANSITION;
            }
            return EVENT_IGNORED;
}
/**************************************************************************************/
event_status_t TIME_SET_ENTRY(protimer_t* const mobj, event_t const * const e) {
    display_time(mobj->curr_time);
    return EVENT_HANDLED;
}
event_status_t TIME_SET_EXIT(protimer_t* const mobj, event_t const * const e) {
    display_clear();
    return EVENT_HANDLED;
}
event_status_t TIME_SET_INC_TIME(protimer_t* const mobj, event_t const * const e) {
    mobj->curr_time += 60;
    display_time(mobj->curr_time);
    return EVENT_HANDLED;
}
event_status_t TIME_SET_DEC_TIME(protimer_t* const mobj, event_t const * const e) {
    if(mobj->curr_time >=60){
       mobj->curr_time -=60;
      display_time(mobj->curr_time);
      return EVENT_HANDLED;
    }
    return EVENT_IGNORED;
}
event_status_t TIME_SET_ABRT(protimer_t* const mobj, event_t const * const e) {
    mobj->active_state = IDLE;
    return EVENT_TRANSITION;
}
event_status_t TIME_SET_START_PAUSE(protimer_t* const mobj, event_t const * const e) {
    if(mobj->curr_time >= 60){
        mobj->active_state =COUNTDOWN;
        return EVENT_TRANSITION;
    }
    return EVENT_IGNORED;
}
/**************************************************************************************/
event_status_t PAUSE_ENTRY(protimer_t* const mobj, event_t const * const e) {
    display_message("Paused",5,1);
    return  EVENT_HANDLED;
}
event_status_t PAUSE_EXIT(protimer_t* const mobj, event_t const * const e) {
    display_clear();
    return EVENT_HANDLED;
}
event_status_t PAUSE_INC_TIME(protimer_t* const mobj, event_t const * const e) {
    mobj->curr_time += 60;
    mobj->active_state = TIME_SET;
    return EVENT_TRANSITION;
}
event_status_t PAUSE_DEC_TIME(protimer_t* const mobj, event_t const * const e) {
    if(mobj->curr_time >= 60){
        mobj->curr_time-=60;
        mobj->active_state = TIME_SET;
        return EVENT_TRANSITION;
    }
    return EVENT_IGNORED;
}
event_status_t PAUSE_START_PAUSE(protimer_t* const mobj, event_t const * const e) {
    mobj->active_state = COUNTDOWN;
    return EVENT_TRANSITION;
}
event_status_t PAUSE_ABRT(protimer_t* const mobj, event_t const * const e) {
    mobj->active_state = IDLE;
    return EVENT_TRANSITION;
}

