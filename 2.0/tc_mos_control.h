#ifndef MOS_CONTROL_H
#define MOS_CONTROL_H

#include<Arduino.h>

#define INVERT_ENABLE false

#define ENABLE 0
#define DISABLE 1

#define OBJECT_NUM 2

extern int object_mode[OBJECT_NUM];
extern int object_former_mode[OBJECT_NUM];
extern byte object_enable[OBJECT_NUM];

enum WorkObject {E0_CAVITY =  0, E1_CAVITY = 1, PLATFORM = 2};
enum ObjectMode {HEATER = 0, COOL = 1, MEMBRANE = 2, NONE = -1};

void control(byte _object, int _object_mode, byte _en);

void mos_init();

#endif
