#ifndef CONFIGARATION_H
#define CONFIGARATION_H

#define STRING_VERSION_CONFIG_H "Last update:" __DATE__ " " __TIME__ // build date and time
#define STRING_CONFIG_H_AUTHOR "EFL-WP" // who made the changes;
#define STRING_VERSION_FIRMWARE_H "Ver:BP6601p-TC.0.1.0.20210316_alpha"

//#define DEBUG

#define BAUDRATE 115200
#define SAMPLE_TIME 10000           // 10ms unit:μs

#define SEMICON_HEAT_MAX_TEMP 60
#define SEMICON_COOL_MIN_TEMP -5
#define PIPE_HEAT_MAX_TEMP 250
#define MEMBRANE_HEAT_MAX_TEMP 100

#define DIRECT 0
#define REVERSE 1

#define WINDOW_SIZE 128

// pid set
// platform cool pid semiconductor
//#define P_COOL_PID {18, 0.04, 0}
#define P_COOL_PID {8.7, 0.06, 0}

// platform heat pid semiconductor
//#define P_HEAT_PID {9, 0.2, 20}
//#define P_HEAT_PID {1.6, 0.04, 0}
#define P_HEAT_PID {1.6, 0.01, 1}

// platform heat pid for membrane
//#define P_PIPE_PID {12, 0.02, 0}
#define P_PIPE_PID {12, 0.1, 0}

// extruder cool pid semiconductor
//#define E_CAVITY_COOL_PID {26, 0.01, 0}
#define E_CAVITY_COOL_PID {6, 0.1, 0}

// extruder heat pid semiconductor
//#define E_CAVITY_HEAT_PID {1.6, 0.1, 0}
#define E_CAVITY_HEAT_PID {1.6, 0.01, 0}

// extruder heat pid for pipe
//#define E_CAVITY_PIPE_PID {4, 0.05, 0}
//#define E_CAVITY_PIPE_PID {2, 0.01, 0}
#define E_CAVITY_PIPE_PID {7, 0.06, 1}

/*
// extruder cool pid semiconductor
#define E0_CAVITY_COOL_PID {26, 0.01, 0}
#define E1_CAVITY_COOL_PID {26, 0.01, 0}

// extruder heat pid semiconductor
#define E0_CAVITY_HEAT_PID {1.6, 0.1, 0}
#define E1_CAVITY_HEAT_PID {1.6, 0.1, 0}

// extruder heat pid for pipe
#define E0_CAVITY_PIPE_PID {4, 0.05, 0}
#define E1_CAVITY_PIPE_PID {4, 0.05, 0}
*/

#endif // CONFIGURATION_H
