#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include"tc_mos_control.h"
#include <Arduino.h>


enum _PID {P = 0, I = 1, D = 2};

extern float e_cavity_cool_pid[3];
extern float e_cavity_heat_pid[3];
extern float e_cavity_pipe_pid[3];
/*
extern float e0_cavity_cool_pid[3];
extern float e0_cavity_heat_pid[3];
extern float e0_cavity_pipe_pid[3];

extern float e1_cavity_cool_pid[3];
extern float e1_cavity_heat_pid[3];
extern float e1_cavity_pipe_pid[3];
*/

extern float p_cool_pid[3];
extern float p_heat_pid[3];
extern float p_pipe_pid[3];

void set_temp(double temp, int _object, int _mode);
double get_temp(byte _object);

double get_set_temp(byte _object);
void report_temperatures();
void auto_report_temperatures();
void marlin_report_temperatures();
void report_temperature();
void report_identify();
void set_auto_report_temp_interval(int _interval);

void set_pid(float _kp, float _ki, float _kd, int _object, int _mode);

bool get_bed_status();

void temp_init();
void tc_manage_heater();

void print_pid(byte _object, byte _mode);
void stop_heat(byte _object);
void stop_all_heat();

double read_100k_thermistor(unsigned long raw);

#endif
