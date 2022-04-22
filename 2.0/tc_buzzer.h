#ifndef BUZZER_H
#define BUZZER_H
#include "arduino.h"

enum Song{APCHY=0,AIR_CITY=1,DIDI=2,ALARM=3};

void buzzer_init();
void play(byte type, int times = 1);

#endif
