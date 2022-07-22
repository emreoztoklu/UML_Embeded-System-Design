#include <Arduino.h>
#include "bsp.h"


void BSP_display(String str){
    Serial.print(str);
}

void BSP_exit(void){
    Serial.print("Bye!!");
}