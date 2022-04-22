#ifndef _TC_WATER_PUMP_H
#define _TC_WATER_PUMP_H
#include "arduino.h"

void pump_init();
void open_pump();
void close_pump();

extern byte pump_status;
extern byte pump_err;
extern float pump_temp;

void manage_pump();

#endif
