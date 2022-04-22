#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include"mos_control.h"
#include <Arduino.h>

//enum HeaterEnum {EXTRUDER = 0 , WATER = 1, PLATFORM = 2};
extern float ec_kp, ec_ki, ec_kd;
extern float eh_kp, eh_ki, eh_kd;
extern float pc_kp, pc_ki, pc_kd;
extern float ph_kp, ph_ki, ph_kd;
extern float e_kp, e_ki, e_kd;
extern float p_kp, p_ki, p_kd;

void set_temp(double temp, int _object, int _mode);
double get_temp(byte flag);
double get_set_temp(byte _object);

void marlin_report_temperatures();

void set_pid(float _kp, float _ki, float _kd, int _object, int _mode);

bool get_bed_status();

void temp_init();
void manage_heater();

void stop_heat(byte _object);

#endif
