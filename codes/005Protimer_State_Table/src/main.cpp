#include "main.h"

static protimer_t protimer;


void setup() {
  Serial.begin(115200);
  //display_init();
  Serial.println("Productive Timer Application");
  Serial.println("============================");

  pinMode(PIN_BUTTON1, INPUT);
  pinMode(PIN_BUTTON2, INPUT);
  pinMode(PIN_BUTTON3, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  protimer_state_table_init(&protimer);
  protimer_init(&protimer);

}

void loop() {
  uint8_t bt_1, bt_2, bt_3, btn_pad_value;
  protimer_user_event_t usr_event;
  static protimer_tick_event_t tck_event;
  static uint32_t current_time = millis();

  //1.read the button pad status
  bt_1 = digitalRead(PIN_BUTTON1);
  bt_2 = digitalRead(PIN_BUTTON2);
  bt_3 = digitalRead(PIN_BUTTON3);

  btn_pad_value = ((bt_1<<2)|(bt_2<<1)|(bt_3));
  /*software button de-bouncing*/
  btn_pad_value = process_bouncing_button_pad_value(btn_pad_value);

  //2.make an event
  if(btn_pad_value){
    if(btn_pad_value == BTN_PAD_VALUE_INC_TIME){
      usr_event.super.sig = INC_TIME;
    }else if(btn_pad_value == BTN_PAD_VALUE_DEC_TIME){
      usr_event.super.sig = DEC_TIME;
    }else if(btn_pad_value == BTN_PAD_VALUE_SP){
      usr_event.super.sig = START_PAUSE;
    }else if(btn_pad_value == BTN_PAD_VALUE_ABRT){
      usr_event.super.sig = ABRT;
    }

    //3.send it to event dispatcher
    protimer_event_dispatcher(&protimer, &usr_event.super);
  }

  // 4.dispatch the time tick event for every 100ms
  if(millis() - current_time >=100){
    current_time = millis();
    tck_event.super.sig = TIME_TICK;
    
    if(++tck_event.ss > 10){
      tck_event.ss = 1;
    }
    protimer_event_dispatcher(&protimer, &tck_event.super);
  }

}


void do_beep(void){
  tone(PIN_BUZZER, 4000, 25);

}


