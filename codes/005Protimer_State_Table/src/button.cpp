#include "button.h"

uint8_t process_bouncing_button_pad_value(uint8_t btn_pad_value){
  // check button each 50ms or 70ms 
  static button_state_t btn_sm_state = NOT_PRESSED;
  static uint32_t curr_time = millis();

  switch (btn_sm_state){
    case NOT_PRESSED:{
      if(btn_pad_value){
        btn_sm_state = BOUNCE;
        curr_time = millis();
      }
      break;
    }
    case BOUNCE:{
      if(millis() - curr_time >= DEBOUNCE_TIME){
        if(btn_pad_value){
          btn_sm_state = PRESSED;
          return btn_pad_value;
        }
        else{
          btn_sm_state = NOT_PRESSED;
        }
      }
      break;
    }
    case PRESSED:{
      if(!btn_pad_value){
        btn_sm_state = BOUNCE;
        curr_time  = millis();
      }
      break;
    }
  }
  return 0;
}