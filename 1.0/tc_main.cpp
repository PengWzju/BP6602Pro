/*
  //======================================================================
  //
  //        Copyright (C) 2019-2020 EFL
  //        All rights reserved
  //
  //        filename : EFL_main.cpp
  //        description :

          chang:
          [20200413] 统一参数关键字为'S'
  //
  //        created by wangpeng at 20200413
  //        http://www.efl-tech.com
  //
  //======================================================================
*/
#include "tc_main.h"
#include "Configuration.h"
#include "temperature.h"
#include<Arduino.h>
#include "mos_control.h"
#include "tc_self_check.h"
#include "buzzer.h"
#include "ConfigurationStore.h"
/*
   ================================== G_Command ========================================
   // G1 -> coordinated movement Y Z
   G28 autohome
   // G4 -> Dwell S<seconds>
   // G90 -> Use Absolute Coordinates
   // G91 -> Use Relative Coordinates


   ================================== M_Command ========================================
   // RepRap/Marlin M Codes
   // M17 - enable/power stepper motors
   // M18 - disable all stepper motors
   // M104 - set extruder target temperature
      INPUT - M104 T*
      RETURN - void
   // M105 - Read current temperature
      INPUT - M105
      return - ET** BT** for extruder & bed current temperature
   // M114 -> Output current position to serail port
   // M140 - set bed target temperature
      INPUT - M140 T*
      RETURN -void

   // Customer M codes
   // M1 - set dlp S<brightness> open dlp dmd, S means brightness
   // M2 -> set led color M2 S<color> open LED, C means color

*/

// --------------- private varibles -------------------
//double MAX_TEMP[NUM_HEATERS] = {EXTRUDER_MAX_TEMP, BED_MAX_TEMP};

//===========================================================================
//=============================public variables=============================
//===========================================================================

//===========================================================================
//=============================private variables=============================
//===========================================================================


// ================ private functions ====================
bool has_command(char key, char input_string[], int string_size);
double search_string(char key, char InputString[], int string_size);

void process_string(char input_string[], int string_size)
{
  int code;

  // find us an M code
  if (has_command('M', input_string, string_size))
  {
    code = search_string('M', input_string, string_size);
    switch (code)
    {
      case 100:
        {
          Serial.println("EFL-BP6602_TC");
          Serial.print("Build time:");
          Serial.println(STRING_VERSION_CONFIG_H);
          Serial.print("Version:");
          Serial.println(STRING_CONFIG_H_VERSION);
        }
        break;
      case 104:
        {
          if (check_extruder() && !check_extruder_max_temp())
          {
            if (has_command('P', input_string, string_size))
            {
              set_temp(search_string('S', input_string, string_size), EXTRUDER, int(search_string('P', input_string, string_size)));
            }
          } else
          {
            Serial.println("Wrong set!Check Extruder");
          }
        }
        break;
      case 140:
        {
          if (check_platform() && !check_platform_max_temp())
          {
            if (has_command('P', input_string, string_size))
            {
              set_temp(search_string('S', input_string, string_size), PLATFORM, search_string('P', input_string, string_size));
            }
          } else
          {
            Serial.println("Wrong set!Check Platform");
          }
        }
        break;
      case 105:
        {
          marlin_report_temperatures();
        }
        break;
      case 106:
        {
          control(FAN, 0, ENABLE);
        }
        break;
      case 107:
        {
          control(FAN, 0, DISABLE);
        }
        break;
      case 245:
        {
          control(FAN, 0, ENABLE);
        }
        break;
      case 246:
        {
          control(FAN, 0, DISABLE);
        }
        break;
      case 301:
        {
          if (has_command('P', input_string, string_size))
            eh_kp = search_string('P', input_string, string_size);
          if (has_command('I', input_string, string_size))
            eh_ki = search_string('I', input_string, string_size);
          if (has_command('D', input_string, string_size))
            eh_kd = search_string('D', input_string, string_size);
          Serial.print("eh_kp:");
          Serial.print(eh_kp, 3);
          Serial.print(" eh_ki:");
          Serial.print(eh_ki, 3);
          Serial.print(" eh_kd:");
          Serial.print(eh_kd, 3);
          Serial.println();
        }
        break;
      case 302:
        {
          if (has_command('P', input_string, string_size))
            ec_kp = search_string('P', input_string, string_size);
          if (has_command('I', input_string, string_size))
            ec_ki = search_string('I', input_string, string_size);
          if (has_command('D', input_string, string_size))
            ec_kd = search_string('D', input_string, string_size);
          Serial.print("ec_kp:");
          Serial.print(ec_kp, 3);
          Serial.print(" ec_ki:");
          Serial.print(ec_ki, 3);
          Serial.print(" ec_kd:");
          Serial.print(ec_kd, 3);
          Serial.println();
        }
        break;
      case 303:
        {
          if (has_command('P', input_string, string_size))
            e_kp = search_string('P', input_string, string_size);
          if (has_command('I', input_string, string_size))
            e_ki = search_string('I', input_string, string_size);
          if (has_command('D', input_string, string_size))
            e_kd = search_string('D', input_string, string_size);
          Serial.print("e_kp:");
          Serial.print(e_kp, 3);
          Serial.print(" e_ki:");
          Serial.print(e_ki, 3);
          Serial.print(" e_kd:");
          Serial.print(e_kd, 3);
          Serial.println();
        }
        break;
      case 304:
        {
          if (has_command('P', input_string, string_size))
            ph_kp = search_string('P', input_string, string_size);
          if (has_command('I', input_string, string_size))
            ph_ki = search_string('I', input_string, string_size);
          if (has_command('D', input_string, string_size))
            ph_kd = search_string('D', input_string, string_size);
          Serial.print("ph_kp:");
          Serial.print(ph_kp, 3);
          Serial.print("ph_ki:");
          Serial.print(ph_ki, 3);
          Serial.print("ph_kd:");
          Serial.print(ph_kd, 3);
          Serial.println();
        }
        break;
      case 305:
        {
          if (has_command('P', input_string, string_size))
            pc_kp = search_string('P', input_string, string_size);
          if (has_command('I', input_string, string_size))
            pc_ki = search_string('I', input_string, string_size);
          if (has_command('D', input_string, string_size))
            pc_kd = search_string('D', input_string, string_size);
          Serial.print("pc_kp:");
          Serial.print(pc_kp, 3);
          Serial.print("pc_ki:");
          Serial.print(pc_ki, 3);
          Serial.print("pc_kd:");
          Serial.print(pc_kd, 3);
          Serial.println();
        }
        break;
      case 306:
        {
          if (has_command('P', input_string, string_size))
            p_kp = search_string('P', input_string, string_size);
          if (has_command('I', input_string, string_size))
            p_ki = search_string('I', input_string, string_size);
          if (has_command('D', input_string, string_size))
            p_kd = search_string('D', input_string, string_size);
          Serial.print("p_kp:");
          Serial.print(p_kp, 3);
          Serial.print("p_ki:");
          Serial.print(p_ki, 3);
          Serial.print("p_kd:");
          Serial.print(p_kd, 3);
          Serial.println();
        }
        break;
      case 500: // M500 Store settings in EEPROM
        {
          Config_StoreSettings();
        }
        break;
      case 501: // M501 Read settings from EEPROM
        {
          Config_RetrieveSettings();
        }
        break;
      case 502: // M502 Revert to default settings
        {
          Config_ResetDefault();
        }
        break;
      case 503: // M503 print settings currently in memory
        {
          Config_PrintSettings();
        }
        break;
      default :
        Serial.print("huh? M");
        Serial.println(code);
    }
  }
}

bool has_command(char key, char InputString[], int string_size) {
  for (byte i = 0; i < string_size ; i++)
  {
    if ((InputString[i] == key))
      return true;
  }
  return false;
}

/*查找对应命令*/
bool has_command_2(char key[], char InputString[], int string_size) {
  for (byte i = 0; i < string_size - 1; i++)
  {
    if ((InputString[i] == key[0]) && (InputString[i + 1] == key[1]))return true;
  }
  return false;
}


/*将命令字符串中的数字转化为double型*/
double search_string(char key, char InputString[], int string_size)
{
  char temp[10] = "";
  for (byte i = 0; i < string_size ; i++)
  {
    if ((InputString[i] == key))
    {
      i += 1;
      int k = 0;
      while (i < string_size && k < 10)
      {
        if (InputString[i] == ' ')
          break;
        temp[k] = InputString[i];
        i++;
        k++;
      }
      return strtod(temp, NULL);
    }
  }
  return 0;
}
