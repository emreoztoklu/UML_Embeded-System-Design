#ifndef MAIN_H
#define MAIN_H

#define PIN_BUTTON1 2       // SET_BUTTON
#define PIN_BUTTON2 3       // OK_BUTTON


#define BTN_PAD_VALUE_SET       2
#define BTN_PAD_VALUE_OK        1
#define BTN_PAD_VALUE_ABRT      3

/**********************************/
#define MCU_SYS_CLOCK_FREQ      16000000UL    //16Mhz system frequency

#define TCCR1A_CTC_MODE         (0U)
#define TCCR1B_CTC_MODE         (1U << 3U)

#define TCCR1B_PRESCALER_1      0x1UL
#define TCCR1B_PRESCALER_8      0x2UL
#define TCCR1B_PRESCALER_64     0x3UL
#define TCCR1B_PRESCALER_256    0x4UL


#define TIMER1_PRESCALER_1      1UL
#define TIMER1_PRESCALER_8      8UL
#define TIMER1_PRESCALER_64     64UL
#define TIMER1_PRESCALER_256    256UL
#define TIMER1_PRESCALER_1024   1024UL


#define CONFIG_TCCR1B_PRESCALER      TCCR1B_PRESCALER_1
#define CONFIG_TIMER1_PRESCALER      TIMER1_PRESCALER_1
#define CONFIG_TICK_PER_SECOND       1000UL


#define MS_PER_TICK                  (1000UL / CONFIG_TICK_PER_SECOND)                  // 1ms 
#define TIMER1_OC_MATCH_VALUE        (((MCU_SYS_CLOCK_FREQ * MS_PER_TICK) / (CONFIG_TIMER1_PRESCALER * 1000UL))-1)
/*Note: UML_Embeded-System-Design\DOCUMENT\OC(output compare val).png*/

//#define MS_TO_TICK(timeInMs)         ((timeInMs * CONFIG_TICK_PER_SECOND) / 1000UL)
#define MS_TO_TICKS( timeInMs )      (( timeInMs) / (MS_PER_TICK) )

/*-------------------------------------------
    CTC Mode:
      Reg:TCCR1A     WGM11:0  WGM10:0
      Reg:TCCR1B     WGM13:0  WGM12:1 
    Prescaler:
      Reg:TCCR1B    CS12: 1  CS11: 0  CS10: 0
    Interrupts:
      Reg:TIMSK1    OCIE1A:1
      Reg:OCR1A     
-------------------------------------------*/
/*-------------------------------------------
  Example:
    Fclk=16Mhz
    Required delay:100ms
    prescaler: 256 

    OCR1A = (Fclk*Reguired_delay/prescaler)-1
-------------------------------------------*/

extern bool flag_report_button_press;

#endif