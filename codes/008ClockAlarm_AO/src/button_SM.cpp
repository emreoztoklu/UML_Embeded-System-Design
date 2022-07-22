/*.$file${AOs::../src::button_SM.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: ClockAlarm.qm
* File:  ${AOs::../src::button_SM.cpp}
*
* This code has been generated by QM 5.1.4 <www.state-machine.com/qm/>.
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*/
/*.$endhead${AOs::../src::button_SM.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#include <Arduino.h>
#include "qpn.h"
#include "button_SM.h"
#include "ClockAlarm_SM.h"
#include "main.h"


/*.$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*. Check for the minimum required QP version */
#if (QP_VERSION < 690U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpn version 6.9.0 or higher required
#endif
/*.$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${AOs::AO_Button} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::AO_Button} .......................................................*/
QActive *const AO_Button =&Button_obj.super;
/*.$enddef${AOs::AO_Button} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${AOs::Button} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Button} ..........................................................*/
Button Button_obj;
/*.${AOs::Button::SM} ......................................................*/
QState Button_initial(Button * const me) {
    /*.${AOs::Button::SM::initial} */
    return Q_TRAN(&Button_Button);
}
/*.${AOs::Button::SM::Button} ..............................................*/
QState Button_Button(Button * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*.${AOs::Button::SM::Button::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            //Info: button_ISR.png

            uint8_t b1, b2, btn_pad_value;

            //1.read the button pad status
            b1 = digitalRead(PIN_BUTTON1);
            b2 = digitalRead(PIN_BUTTON2);

            btn_pad_value = ((b1 << 1UL) | b2);

            if(btn_pad_value){
                if(btn_pad_value == BTN_PAD_VALUE_SET){
                    QACTIVE_POST(AO_ClockAlarm, SET_SIG, 0U);
                }
                else if(btn_pad_value == BTN_PAD_VALUE_OK){
                    QACTIVE_POST(AO_ClockAlarm, OK_SIG, 0U);
                }
                else if(btn_pad_value == BTN_PAD_VALUE_ABRT){
                    QACTIVE_POST(AO_ClockAlarm, ABRT_SIG, 0U);
                }
            }

            QF_INT_DISABLE();
            flag_report_button_press = true;
            QF_INT_ENABLE();


            // Note: Active_POST
            //https://www.state-machine.com/qpn/qfn_8h.html#ac41ac57f925509fc92f9b3612f8c03b5
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*.$enddef${AOs::Button} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${AOs::Button_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Button_ctor} .....................................................*/
void Button_ctor(void) {
    QActive_ctor(AO_Button, Q_STATE_CAST(&Button_initial));
}
/*.$enddef${AOs::Button_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

