#include "usart.h"
#include <Arduino.h>
#include"temperature.h"
#include "efl_serial.h"
#include "Configuration.h"

#define EXTRUDER_CUR_TEMP "ext_cur.txt=\""
#define EXTRUDER_CUR_TEMP_NONE "ext_cur.txt=\"-\""
#define EXTRUDER_TAR_TEMP "ext_tar.txt=\""

#define PLATFORM_CUR_TEMP "plat_cur.txt=\""
#define PLATFORM_CUR_TEMP_NONE "plat_cur.txt=\"-\""
#define PLATFORM_TAR_TEMP "plat_tar.txt=\""
#define WATER_CUR_TEMP "water_temp.txt=\""
#define WATER_CUR_TEMP_NONE "water_temp.txt=\"-\""

#define WATER_GOOD "home.q_water.picc=0"
#define WATER_WARNING "home.q_water.picc=1"
#define WATER_BAD "home.q_water.picc=2"
#define WATER_LEVEL_LOW_J "home.water_level.val=10"
#define WATER_LEVEL_MEDIUM_J "home.water_level.val=50"
#define WATER_LEVEL_HIGH_J "home.water_level.val=90"

int ui_refresh_count = 0;

void print_command_string(const char str[]);
void code_end();

unsigned long ui_time = 0;

void update_ui()
{
#ifdef USART_CONTROLLER
  if ((millis() - ui_time) >= UI_REFRESH_INTERVAL * 1000)
  {
    ui_time = millis();
    double extruder_cur_temp = get_temp(EXTRUDER);
    if (extruder_cur_temp > -30)
    {
      print_txt_string(EXTRUDER_CUR_TEMP, extruder_cur_temp, 1);
    } else
    {
      print_command_string(EXTRUDER_CUR_TEMP_NONE);
    }
    double platform_cur_temp = get_temp(PLATFORM);
    if (platform_cur_temp > -30)
    {
      print_txt_string(PLATFORM_CUR_TEMP, platform_cur_temp, 1);
    } else
    {
      print_command_string(PLATFORM_CUR_TEMP_NONE);
    }
    ui_refresh_count = 0;
  }
#endif
}

void print_txt_string(char string[], double value, byte floatbit = 1)
{
  Serial.print(string);
  Serial.print(value, floatbit);
  Serial.print("\"");
  code_end();
}

void print_txt(char string[], char com[])
{
  Serial.print(string);
  Serial.print(com);
  Serial.print("\"");
  code_end();
}

void print_command_string(const char str[])
{
  Serial.print(str);
  code_end();
}

void code_end()
{
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}
