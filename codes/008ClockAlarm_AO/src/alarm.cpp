/*.$file${AOs::../src::alarm.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: ClockAlarm.qm
* File:  ${AOs::../src::alarm.cpp}
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
/*.$endhead${AOs::../src::alarm.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#include <Arduino.h>
#include "qpn.h"
#include "alarm.h"
#include "ClockAlarm_SM.h"

Q_DEFINE_THIS_FILE;

/*.$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*. Check for the minimum required QP version */
#if (QP_VERSION < 690U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpn version 6.9.0 or higher required
#endif
/*.$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${AOs::Alarm} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Alarm} ...........................................................*/
/*.${AOs::Alarm::init} .....................................................*/
void Alarm_init(Alarm * const me) {
    QHSM_INIT(&me->super);
}

/*.${AOs::Alarm::dispatch} .................................................*/
void Alarm_dispatch(Alarm * const me) {
    QHSM_DISPATCH(&me->super);
}

/*.${AOs::Alarm::set_alarm_time} ...........................................*/
void Alarm_set_alarm_time(Alarm * const me, uint32_t alarm_time) {
    me->alarm_time = alarm_time;
}

/*.${AOs::Alarm::set_status} ...............................................*/
void Alarm_set_status(Alarm * const me, uint8_t alarm_status) {
    me->alarm_status = alarm_status;
}

/*.${AOs::Alarm::get_alarm_time} ...........................................*/
uint32_t Alarm_get_alarm_time(Alarm * const me) {
    return me->alarm_time;
}

/*.${AOs::Alarm::SM} .......................................................*/
QState Alarm_initial(Alarm * const me) {
    /*.${AOs::Alarm::SM::initial} */
    me->alarm_time = INITIAL_ALARM_TIME;
    me->alarm_status = ALARM_OFF;
    return Q_TRAN(&Alarm_ALARM);
}
/*.${AOs::Alarm::SM::ALARM} ................................................*/
QState Alarm_ALARM(Alarm * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /*.${AOs::Alarm::SM::ALARM::ALARM_CHECK} */
        case ALARM_CHECK_SIG: {

            if(Q_PAR(me) == me->alarm_time)
                QACTIVE_POST(AO_ClockAlarm, ALARM_SIG, 0U);

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
/*.$enddef${AOs::Alarm} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${AOs::Alarm_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Alarm_ctor} ......................................................*/
void Alarm_ctor(Alarm *const me) {
    QHsm_ctor(&me->super, Q_STATE_CAST(&Alarm_initial));
}
/*.$enddef${AOs::Alarm_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
