#include <main.h>
#include <lcd.h>

#define PUBLIC
#define PRIVATE static

PRIVATE event_status_t protimer_state_handler_IDLE(protimer_t * const mobj, event_t const * const e);
PRIVATE event_status_t protimer_state_handler_COUNTDOWN(protimer_t* const mobj, event_t const * const e);
PRIVATE event_status_t protimer_state_handler_TIME_SET(protimer_t* const mobj, event_t const * const e);
PRIVATE event_status_t protimer_state_handler_PAUSE(protimer_t* const mobj, event_t const * const e);
PRIVATE event_status_t protimer_state_handler_STAT(protimer_t* const mobj, event_t const * const e);


void protimer_init(protimer_t* mobj){
    event_t event_enter_action;
    event_enter_action.sig = ENTRY;
    mobj->active_state = IDLE;
    mobj->pro_time = 0;
    protimer_state_machine(mobj, &event_enter_action);
}

void protimer_event_dispatcher(protimer_t* const mobj, event_t const * const e){
    protimer_states_t source, target;     //dummy variable
    event_status_t status;
    
    source = mobj->active_state;
    status = protimer_state_machine(mobj, e);

    if (status == EVENT_TRANSITION){
      target = mobj->active_state;
      event_t ee;
      //1.run exit action for the source state
      ee.sig = EXIT;
      mobj->active_state = source;
      protimer_state_machine(mobj, &ee);

      //2. run the entry action for the target state
      ee.sig = ENTRY;
      mobj->active_state = target;
      protimer_state_machine(mobj, &ee);
    }
}

event_status_t protimer_state_machine(protimer_t* const mobj, event_t const * const e ){
    switch (mobj->active_state){
        case IDLE:
            return protimer_state_handler_IDLE(mobj, e);
        case TIME_SET:
            return protimer_state_handler_TIME_SET(mobj, e);
        case COUNTDOWN:
            return protimer_state_handler_COUNTDOWN(mobj, e);
        case PAUSE:
            return protimer_state_handler_PAUSE(mobj, e);
        case STAT:
            return protimer_state_handler_STAT(mobj, e);
    } //END of Switch
    return EVENT_IGNORED;
}
PRIVATE event_status_t protimer_state_handler_IDLE(protimer_t* const mobj, event_t const * const e) {
    switch (e->sig){
        case ENTRY:{
            mobj->curr_time = 0;
            mobj->elapsed_time = 0;
            display_time(0);
            display_message("Set",0,0);
            display_message("Time",0,1);
            return EVENT_HANDLED;
        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;
        }
        case INC_TIME:{
            mobj->curr_time +=60;
            mobj->active_state = TIME_SET;
            return EVENT_TRANSITION;
        }
        case START_PAUSE:{
            mobj->active_state = STAT;
            return EVENT_TRANSITION;
        }

        case TIME_TICK:{
            if(((protimer_tick_event_t*)(e))->ss == 5){
                do_beep();
                return EVENT_HANDLED;
            }
            
            return EVENT_IGNORED;  
        }
    } //END of Switch
     return EVENT_IGNORED;    
}
PRIVATE event_status_t protimer_state_handler_COUNTDOWN(protimer_t* const mobj, event_t const * const e){
    static uint8_t tick_count;
    switch (e->sig){
        case EXIT:{
            mobj->pro_time += mobj->elapsed_time;
            mobj->elapsed_time = 0;
            return EVENT_HANDLED;
        }
        case TIME_TICK:{
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
        case START_PAUSE:{
            mobj->active_state = PAUSE;
            return EVENT_TRANSITION;
        }
        case ABRT:{
            mobj->active_state = IDLE;
            return EVENT_TRANSITION;
        }
    }
    return EVENT_IGNORED;
}
PRIVATE event_status_t protimer_state_handler_STAT(protimer_t* const mobj, event_t const * const e){
    static uint8_t tick_count;
    switch (e->sig){
        case ENTRY:{
            display_time(mobj->pro_time);
            display_message("Productive Time",1,1);
            return EVENT_HANDLED;
        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;
        }
        case TIME_TICK:{
            if(++tick_count == 30){
                tick_count = 0;
                mobj->active_state = IDLE;
                return EVENT_TRANSITION;
            }
            return EVENT_IGNORED;
        }
    }
    return EVENT_IGNORED;
}
PRIVATE event_status_t protimer_state_handler_TIME_SET(protimer_t* const mobj, event_t const * const e){
    switch (e->sig){
        case ENTRY:{
            display_time(mobj->curr_time);
            return EVENT_HANDLED;
        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;
        }
        case INC_TIME:{
            mobj->curr_time += 60;
            display_time(mobj->curr_time);
            return EVENT_HANDLED;
        }
        case DEC_TIME:{
            if(mobj->curr_time >=60){
                mobj->curr_time -=60;
                display_time(mobj->curr_time);
                return EVENT_HANDLED;
            }
            return EVENT_IGNORED;
        }
        case ABRT:{
            mobj->active_state = IDLE;
            return EVENT_TRANSITION;
        }
        case START_PAUSE:{
            if(mobj->curr_time >= 60){
                mobj->active_state =COUNTDOWN;
                return EVENT_TRANSITION;
            }
            return EVENT_IGNORED;
        }
    }
    return EVENT_IGNORED;
}
PRIVATE event_status_t protimer_state_handler_PAUSE(protimer_t* const mobj, event_t const * const e){
    switch (e->sig){
        case ENTRY:{
            display_message("Paused",5,1);
            return  EVENT_HANDLED;
        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;
        }
        case INC_TIME:{
            mobj->curr_time += 60;
            mobj->active_state = TIME_SET;
            return EVENT_TRANSITION;
        }
        case DEC_TIME:{
            if(mobj->curr_time >= 60){
                mobj->curr_time-=60;
                mobj->active_state = TIME_SET;
                return EVENT_TRANSITION;
            }
            return EVENT_IGNORED;
        }
        case START_PAUSE:{
            mobj->active_state = COUNTDOWN;
            return EVENT_TRANSITION;
        }
        case ABRT:{
            mobj->active_state = IDLE;
            return EVENT_TRANSITION;
        }    
    }
    return EVENT_IGNORED;
}
