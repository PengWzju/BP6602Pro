#ifndef CONFIGARATION_H
#define CONFIGARATION_H

#define STRING_VERSION_CONFIG_H __DATE__ " " __TIME__ // build date and time
#define STRING_CONFIG_H_VERSION "BP6602p-TC.0.1.6.20210912"

//#define DEBUG

#define BAUDRATE 115200
#define SAMPLE_TIME 10000           // 10ms

#define EXTRUDER_MAX_TEMP 70
#define PLATFORM_MAX_TEMP 70
//#define WATER_MAX_TEMP 50

#define EXTRUDER_MIN_TEMP -5
#define PLATFORM_MIN_TEMP -5

#define AUTOMATIC  1
#define MANUAL 0
#define DIRECT 0
#define REVERSE 1

#define WINDOW_SIZE 128

//#define USART_CONTROLLER
#ifdef USART_CONTROLLER
#define UI_REFRESH_INTERVAL 3
#endif

// pid set
// platform cool pid semiconductor
#define PC_KP 15
#define PC_KI 0.1
#define PC_KD 0
// platform heat pid semiconductor
#define PH_KP 7
#define PH_KI 0.03
#define PH_KD 0
// platform heat pid for t2
#define P_KP 3.9
#define P_KI 0.015
#define P_KD 0  

// extruder cool pid semiconductor
#define EC_KP 8
#define EC_KI 0.1
#define EC_KD 0
// extruder heat pid semiconductor
#define EH_KP 3
#define EH_KI 0.02
#define EH_KD 0
// extruder heat pid for t2
#define E_KP 2.9
#define E_KI 0.02
#define E_KD 0

#define EEPROM_CHITCHAT
#define EEPROM_SETTINGS

#endif // CONFIGURATION_H
