#include <Arduino.h>
#include "qpn.h"
#include "QHSM_Test.h"
Q_DEFINE_THIS_FILE;

void setup() {

  Serial.begin(115200);
  Serial.println("QHSM Testing...");
  
  QHsmTst_ctor();
  QHSM_INIT(super_QHsmTst);
  Serial.print('\n');
}

void loop() {

  char user_event;

  if(Serial.available() > 0){
    user_event = Serial.read();

    if(user_event == 'a' || user_event =='A'){
      super_QHsmTst->evt.sig = (QSignal)A_SIG;
    }
    else if(user_event == 'b' || user_event == 'B') {Q_SIG(super_QHsmTst) = (QSignal)B_SIG;}
    else if(user_event == 'c' || user_event == 'C') {Q_SIG(super_QHsmTst) = (QSignal)C_SIG;} 
    else if(user_event == 'd' || user_event == 'D') {Q_SIG(super_QHsmTst) = (QSignal)D_SIG;} 
    else if(user_event == 'e' || user_event == 'E') {Q_SIG(super_QHsmTst) = (QSignal)E_SIG;} 
    else if(user_event == 'f' || user_event == 'F') {Q_SIG(super_QHsmTst) = (QSignal)F_SIG;} 
    else if(user_event == 'g' || user_event == 'G') {Q_SIG(super_QHsmTst) = (QSignal)G_SIG;} 
    else if(user_event == 'h' || user_event == 'H') {Q_SIG(super_QHsmTst) = (QSignal)H_SIG;} 
    else if(user_event == 'i' || user_event == 'I') {Q_SIG(super_QHsmTst) = (QSignal)I_SIG;} 
    else if(user_event == 'x' || user_event == 'X') {Q_SIG(super_QHsmTst) = (QSignal)TERMINATE_SIG;}
    else Q_SIG(super_QHsmTst) = IGNORE_SIG;

    QHSM_DISPATCH(super_QHsmTst);
    Serial.print('\n');

  } 
}

Q_NORETURN Q_onAssert	(char_t const Q_ROM *const module,int_t const location ){
  Serial.println("---Assertion Failure!!---");
  Serial.println((String)module);
  Serial.println(location);
}	

