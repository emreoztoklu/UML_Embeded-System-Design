#include <Arduino.h>
#include "qpn.h"
#include "ClockAlarm_SM.h"
#include "lcd.h"
#include "main.h"

Q_DEFINE_THIS_FILE;


static void Timer1_init(void);
uint8_t process_button_pad_value(uint8_t btn_pad_value);


void setup() {
  Serial.begin(115200);
  display_init();
  Clock_Alarm_ctor();

  pinMode(PIN_BUTTON1, INPUT);
  pinMode(PIN_BUTTON2, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  QHSM_INIT(super_ClockAlarm);

  Timer1_init();
}

void loop() {
  static uint32_t tick_time=millis();
  static uint32_t alarm_check_time = millis();
  uint8_t b1, b2, btn_pad_value;

  //1: read the button pad status
  b1 = digitalRead(PIN_BUTTON1);
  b2 = digitalRead(PIN_BUTTON2);

  btn_pad_value = ((b1 << 1U) | b2);

  //software button de-bouncing
  btn_pad_value = process_button_pad_value(btn_pad_value);

 /****************************************/
  while(millis() - tick_time >= 50){
    //send TICK event
    tick_time = millis();

    Q_SIG(super_ClockAlarm) = TICK_SIG;
    QHSM_DISPATCH(super_ClockAlarm);
  }
 /****************************************/
  while (millis() - alarm_check_time >= 500){
    //send TICK event
    alarm_check_time = millis();

    Q_SIG(super_ClockAlarm) = ALARM_SIG;
    QHSM_DISPATCH(super_ClockAlarm);
  }
 /****************************************/
  if(btn_pad_value){

    if(btn_pad_value == BTN_PAD_VALUE_SET){
      Q_SIG(super_ClockAlarm) = SET_SIG;
    }
    else if (btn_pad_value == BTN_PAD_VALUE_OK){
      Q_SIG(super_ClockAlarm) = OK_SIG;
    }

    else if (btn_pad_value == BTN_PAD_VALUE_ABRT){
      Q_SIG(super_ClockAlarm) = ABRT_SIG;
    }
    else  {
      Q_SIG(super_ClockAlarm) = IGNORE_SIG;
    }

    QHSM_DISPATCH(super_ClockAlarm);
  }
 /****************************************/
}

Q_NORETURN Q_onAssert	(char_t const Q_ROM *const module,int_t const location ){
  Serial.println("Assertion Failure!!");
  Serial.println((String)module);
  Serial.println(location);
  while(1);
}	


static void Timer1_init(void){
  /*CTC Mode:
      Reg:TCCR1A     WGM11:0  WGM10:0
      Reg:TCCR1B     WGM13:0  WGM12:1 
    Prescaler:
      Reg:TCCR1B    CS12: 1  CS11: 0  CS10: 0
    Interrupts:
      Reg:TIMSK1    OCIE1A:1
      Reg:OCR1A     
  */

  TCCR1A = 0;          //CTC mode
  TCCR1B = B00001100;  //12- prescaler=256, CTC mode
  TIMSK1 |= B00000010; //2 - Interrupt enable for OCR1A compare match
  OCR1A = 6250-1;      //OC match value for 100ms time base generation 
  /*
    Fclk=16Mhz
    Required delay:100ms
    prescaler: 256 

    OCR1A = (Fclk*Reguired_delay/prescaler)-1
  */
}


uint8_t process_button_pad_value(uint8_t bttn_value) {
  static button_state_t btn_sm_state = NOT_PRESSED;
  static uint32_t curr_time = millis();

  switch(btn_sm_state){
    case NOT_PRESSED:{
      if(bttn_value){
        btn_sm_state = BOUNCE;
        curr_time = millis();
      }
      break;
    }
    case BOUNCE:{
      if(millis() - curr_time >= 50 ){
        //50ms has passed 
        if(bttn_value){
          btn_sm_state = PRESSED;
          return bttn_value;
        }
        else
          btn_sm_state = NOT_PRESSED;
      }
      break;
    }
    case PRESSED:{
      if(!bttn_value){
        btn_sm_state = BOUNCE;
        curr_time = millis();
      }
      break;
    }

  }

  return 0;
}


void do_beep(void){
  tone(PIN_BUZZER, 4000, 25);
}
