#ifndef PINS_H
#define PINS_H

#define X_MS1_PIN -1
#define X_MS2_PIN -1
#define Y_MS1_PIN -1
#define Y_MS2_PIN -1
#define Z_MS1_PIN -1
#define Z_MS2_PIN -1
#define E0_MS1_PIN -1
#define E0_MS2_PIN -1
#define E1_MS1_PIN -1
#define E1_MS2_PIN -1
#define DIGIPOTSS_PIN -1


/****************************************************************************************
* Gen7 v1.1, v1.2, v1.3 pin assignment
*
****************************************************************************************/


/****************************************************************************************
* Gen7 v1.4 pin assignment
*
****************************************************************************************/



/*******************************************************************************
*********
* Gen7 Alfons3  pin assignment
*
********************************************************************************
********/
/* These Pins are assigned for the modified GEN7 Board from Alfons3 Please review the pins and adjust it for your needs*/


/****************************************************************************************
* Arduino Mega pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 3 || MOTHERBOARD == 33 || MOTHERBOARD == 34
#define KNOWN_BOARD 1

//////////////////FIX THIS//////////////
#ifndef __AVR_ATmega1280__
 #ifndef __AVR_ATmega2560__
 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
 #endif
#endif

// uncomment one of the following lines for RAMPS v1.3 or v1.0, comment both for v1.2 or 1.1
// #define RAMPS_V_1_3
// #define RAMPS_V_1_0

#if MOTHERBOARD == 33 || MOTHERBOARD == 34

#define LARGE_FLASH        true

#define X_STEP_PIN         14 // 54->4
#define X_DIR_PIN          15  // 55->5
#define X_ENABLE_PIN       38
#define X_MIN_PIN          23
#define X_MAX_PIN          22

#define Y_STEP_PIN         21 // 60->45
#define Y_DIR_PIN          32 // 61->32
#define Y_ENABLE_PIN       15 // 56->11
//#define Y_STEP_PIN         36
//#define Y_DIR_PIN          34
//#define Y_ENABLE_PIN       30
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         36
#define Z_DIR_PIN          38
#define Z_ENABLE_PIN       37 // 62->43
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define Z2_STEP_PIN        36
#define Z2_DIR_PIN         34
#define Z2_ENABLE_PIN      34

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      34

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#if MOTHERBOARD == 33
#define FAN_PIN            50 // (Sprinter config)
#else
#define FAN_PIN            14 // IO pin. Buffer needed
#endif
#define PS_ON_PIN          12

#if defined(REPRAP_DISCOUNT_SMART_CONTROLLER) || defined(G3D_PANEL)
#define KILL_PIN           -1
#else
#define KILL_PIN           -1
#endif

#define HEATER_0_PIN       17   // EXTRUDER 1
#define HEATER_1_PIN       27  // 7->27 wp change 20210517
#define HEATER_2_PIN       -1
#define TEMP_0_PIN         0   // ANALOG NUMBERING // wp change 20210513
#define TEMP_1_PIN         2   // ANALOG NUMBERING
#define TEMP_2_PIN         -1   // ANALOG NUMBERING  // wp changed 20210517
#define HEATER_BED_PIN     17    // BED  // 9->31 WP changed 20210517
#define TEMP_BED_PIN       4   // ANALOG NUMBERING   

// for 0.3.2
#define TEMP_SWITCH_E0_PIN 45
#define TEMP_SWITCH_E1_PIN 46
#define TEMP_SWITCH_B_PIN 47

#else // RAMPS_V_1_1 or RAMPS_V_1_2 as default (MOTHERBOARD == 3)


#endif// MOTHERBOARD == 33 || MOTHERBOARD == 34

// SPI for Max6675 Thermocouple

#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support
  #define MAX_SCK_PIN          52
  #define MAX_MISO_PIN         50
  #define MAX_MOSI_PIN         51
  #define MAX6675_SS       53
#else
  #define MAX6675_SS       49
#endif

#endif//MOTHERBOARD == 3 || MOTHERBOARD == 33 || MOTHERBOARD == 34

/****************************************************************************************
* Duemilanove w/ ATMega328P pin assignment
*
****************************************************************************************/


/****************************************************************************************
* Gen6 pin assignment
*
****************************************************************************************/


/****************************************************************************************
* Sanguinololu pin assignment
*
****************************************************************************************/

/*****************************************************************
* Ultimaker pin assignment
******************************************************************/

/*****************************************************************
* Ultimaker pin assignment (Old electronics)
******************************************************************/


/****************************************************************************************
* RUMBA pin assignment
*
****************************************************************************************/


/****************************************************************************************
* Teensylu 0.7 / Printrboard pin assignments (AT90USB1286)
* Requires the Teensyduino software with Teensy++ 2.0 selected in Arduino IDE!
  http://www.pjrc.com/teensy/teensyduino.html
* See http://reprap.org/wiki/Printrboard for more info
****************************************************************************************/


/****************************************************************************************
 * Brainwave 1.0 pin assignments (AT90USB646)
 * Requires hardware bundle for Arduino:
   https://github.com/unrepentantgeek/brainwave-arduino
 ****************************************************************************************/


/****************************************************************************************
* Gen3+ pin assignment
*
****************************************************************************************/


/****************************************************************************************
* Open Motion controller with enable based extruders
*
*                        ATMega644
*
*                        +---\/---+
*            (D 0) PB0  1|        |40  PA0 (AI 0 / D31)
*            (D 1) PB1  2|        |39  PA1 (AI 1 / D30)
*       INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D29)
*        PWM (D 3) PB3  4|        |37  PA3 (AI 3 / D28)
*        PWM (D 4) PB4  5|        |36  PA4 (AI 4 / D27)
*       MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26)
*       MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25)
*        SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D24)
*                  RST  9|        |32  AREF
*                  VCC 10|        |31  GND
*                  GND 11|        |30  AVCC
*                XTAL2 12|        |29  PC7 (D 23) 
*                XTAL1 13|        |28  PC6 (D 22)
*       RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
*       TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
*  INT0 RX1 (D 10) PD2 16|        |25  PC3 (D 19) TMS
*  INT1 TX1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
*       PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
*       PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
*       PWM (D 14) PD6 20|        |21  PD7 (D 15) PWM
*                        +--------+
*
****************************************************************************************/

/*****************************************************************
* Rambo Pin Assignments
******************************************************************/

/****************************************************************************************
* MegaTronics
*
****************************************************************************************/


/****************************************************************************************
* MegaTronics v2.0
*
****************************************************************************************/


/****************************************************************************************
* Minitronics v1.0
*
****************************************************************************************/




#ifndef KNOWN_BOARD
#error Unknown MOTHERBOARD value in configuration.h
#endif

//List of pins which to ignore when asked to change by gcode, 0 and 1 are RX and TX, do not mess with those!
#define _E0_PINS E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, HEATER_0_PIN,
#if EXTRUDERS > 1
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN, HEATER_1_PIN,
#else
  #define _E1_PINS
#endif
#if EXTRUDERS > 2
  #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN, HEATER_2_PIN,
#else
  #define _E2_PINS
#endif

#ifdef X_STOP_PIN
  #if X_HOME_DIR < 0
    #define X_MIN_PIN X_STOP_PIN
    #define X_MAX_PIN -1
  #else
    #define X_MIN_PIN -1
    #define X_MAX_PIN X_STOP_PIN
  #endif
#endif

#ifdef Y_STOP_PIN
  #if Y_HOME_DIR < 0
    #define Y_MIN_PIN Y_STOP_PIN
    #define Y_MAX_PIN -1
  #else
    #define Y_MIN_PIN -1 
    #define Y_MAX_PIN Y_STOP_PIN
  #endif
#endif

#ifdef Z_STOP_PIN
  #if Z_HOME_DIR < 0
    #define Z_MIN_PIN Z_STOP_PIN
    #define Z_MAX_PIN -1
  #else
    #define Z_MIN_PIN -1
    #define Z_MAX_PIN Z_STOP_PIN
  #endif
#endif

#ifdef DISABLE_MAX_ENDSTOPS
#define X_MAX_PIN          -1
#define Y_MAX_PIN          -1
#define Z_MAX_PIN          -1
#endif

#define SENSITIVE_PINS {0, 1, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, PS_ON_PIN, \
                        HEATER_BED_PIN, FAN_PIN,                  \
                        _E0_PINS _E1_PINS _E2_PINS             \
                        analogInputToDigitalPin(TEMP_0_PIN), analogInputToDigitalPin(TEMP_1_PIN), analogInputToDigitalPin(TEMP_2_PIN), analogInputToDigitalPin(TEMP_BED_PIN) }
#endif
