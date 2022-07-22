#include <Arduino.h>
#include "qpn.h"
#include "ClockAlarm_SM.h"
#include "button_SM.h"
#include "lcd.h"
#include "main.h"

Q_DEFINE_THIS_FILE;

bool flag_report_button_press = true;

static void sys_tick_init(void);
static void attach_button_interrupts(void);

/*************************************************************************/
/* DOCUMENT->multiple_SM->10_AO_framework.png*/
/*1_ Create Events Queue (array of QEvt) for the objects */
static QEvt ClockAlarm_Queue[5];
static QEvt Button_Queue[5];

/*2_ Create and initalzie Active object control block (QActiveCB)*/
QActiveCB const QF_active[]={
  {(QActive*)0, (QEvt*)0, 0},
  {(QActive*)AO_ClockAlarm, (QEvt*)ClockAlarm_Queue, Q_DIM(ClockAlarm_Queue)},
  {(QActive*)AO_Button, (QEvt*)Button_Queue, Q_DIM(Button_Queue)},
};
/*3_ Call QF_init(maxActive) to initalize the Active object framework of QP-Nano       Check: void setup()*/
/*4_ Call QF_run()                                                                     Check: void loop() */    
/*5_ Call QF_tickXISR()  from your application's tick ISR                               Check: ISR(TIMER1_COMPA_vect) in ClockAlarm.cpp*/
  //Info: [tickXISR] https://www.state-machine.com/qpn/qfn_8h.html#a48f78cdad2d12bbbd2fc584bab1c56c2
/*************************************************************************/

void setup() {
  Serial.begin(115200);
  display_init();

  /********************/
  attach_button_interrupts();
 
  /********************/

  Clock_Alarm_ctor();
  Button_ctor();
  /*3_ Call QF_init(maxActive) to initalize the Active object framework of QP-Nano*/
  QF_init(Q_DIM(QF_active));
 
  /********************/

  
}

void loop() {
/*4_ Call QF_run()*/
  QF_run();
}

void QF_onStartup(void){
  sys_tick_init();
}

/*This function related with QF_run, When it goes IDLE mode in sleep mode it should come this function*/
void QV_onIdle(void){
  /*qfn_port.h*/
  QV_CPU_SLEEP();
}

void SET_Handler(void){
  QF_INT_DISABLE();           // Stop Interrupts cli();
  
  if(flag_report_button_press) {
    flag_report_button_press = false;
    QActive_armX(AO_Button, 0, MS_TO_TICKS(50), 0U );   // 50ms
    // https://www.state-machine.com/qpn/struct_q_active.html#adbc1b10b5546d6441a823da4867f4692
  }

  QF_INT_ENABLE();           // Enable Interrupts cli();
}

void OK_Handler(void) {
  QF_INT_DISABLE();           // Stop Interrupts cli();
  
  if(flag_report_button_press) {
    flag_report_button_press = false;
    QActive_armX(AO_Button, 0, MS_TO_TICKS(50), 0U );   // 50ms
  }

  QF_INT_ENABLE();           // Enable Interrupts cli();
}

static void attach_button_interrupts(void){
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON1), SET_Handler, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON2), OK_Handler,  RISING);
}

Q_NORETURN Q_onAssert	(char_t const Q_ROM *const module,int_t const location ){
  Serial.println("Assertion Failure!!");
  Serial.println((String)module);
  Serial.println(location);
  while(1);
}	

static void sys_tick_init(void){
  TCCR1A = TCCR1A_CTC_MODE;                           //CTC mode            
  TCCR1B = (TCCR1B_CTC_MODE | TCCR1B_PRESCALER_1);    //CTC mode, prescaler=1
  TIMSK1 |= B00000010;                                //Interrupt enable for OCR1A compare match
  OCR1A = TIMER1_OC_MATCH_VALUE;                      //OC match value for CONFIG_TICKS_PER_SECOND time base generation
}

