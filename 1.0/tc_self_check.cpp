#include "tc_self_check.h"
#include "temperature.h"
#include "pins.h"
#include "buzzer.h"
#include "mos_control.h"

#define SEMICON_HEAT_ALARM_TEMP 80
#define SEMICON_COOL_ALARM_TEMP 30
#define MEMBRANE_ALARM_TEMP 110
bool platform_on = false;
bool extruder_on = false;
bool platform_max_temp = false;
bool extruder_max_temp = false;

void check_init()
{
  extruder_on = check_extruder();
  platform_on = check_platform();
  if (extruder_on)
    play(DIDI);
  if (platform_on)
    play(DIDI);
}

void max_temp_check()
{
  if (extruder_on)
  {
    extruder_on = check_extruder();
    double _e_temp = get_temp(EXTRUDER);
    if (!extruder_on)
    {
      stop_heat(EXTRUDER);
      Serial.println("EXTRUDER Take Off");
      play(ALARM);
    }

    if (object_mode[EXTRUDER] == MEMBRANE)
    {
      if (_e_temp > MEMBRANE_ALARM_TEMP)
      {
        stop_heat(EXTRUDER);
        play(ALARM);
        Serial.print(_e_temp);
        Serial.println("Extruder max temp triggered!");
        extruder_max_temp = true;
      } else
      {
        extruder_max_temp = false;
      }
    } else if ((object_mode[EXTRUDER] == HEATER))
    {
      if (_e_temp > SEMICON_HEAT_ALARM_TEMP)
      {
        stop_heat(EXTRUDER);
        play(ALARM);
        Serial.print(_e_temp);
        Serial.println("Extruder SemiCon Heat max temp triggerred!");
        extruder_max_temp = true;
      } else
      {
        extruder_max_temp = false;
      }
    } else if ((object_mode[EXTRUDER] == COOL))
    {
      if (_e_temp > SEMICON_COOL_ALARM_TEMP)
      {
        stop_heat(EXTRUDER);
        play(ALARM);
        Serial.print(_e_temp);
        Serial.println("Extruder Semicon Cool max temp triggerred!");
        extruder_max_temp = true;
      } else
      {
        extruder_max_temp = false;
      }
    }
  } else
  {
    extruder_on = check_extruder();
    if (extruder_on)
    {
      Serial.println("EXTRUDER Put On");
      play(DIDI);
    }
  }
  if (platform_on)
  {
    platform_on = check_platform();
    double _p_temp = get_temp(PLATFORM);
    if (!platform_on)
    {
      stop_heat(PLATFORM);
      Serial.println("PLATFORM Take Off");
      play(ALARM);
    }
    if (object_mode[PLATFORM] == MEMBRANE)
    {
      if (_p_temp > MEMBRANE_ALARM_TEMP)
      {
        stop_heat(PLATFORM);
        play(ALARM);
        Serial.print(_p_temp);
        Serial.println("PLATFORM max temp triggerred!");
        platform_max_temp = true;
      } else
      {
        platform_max_temp = false;
      }
    } else if ((object_mode[PLATFORM] == HEATER))
    {
      if (_p_temp > SEMICON_HEAT_ALARM_TEMP)
      {
        stop_heat(PLATFORM);
        play(ALARM);
        Serial.print(_p_temp);
        Serial.println("PLATFORM SemiCon max temp triggerred!");
        platform_max_temp = true;
      } else
      {
        platform_max_temp = false;
      }
    } else if ((object_mode[PLATFORM] == COOL))
    {
      if (_p_temp > SEMICON_COOL_ALARM_TEMP)
      {
        stop_heat(PLATFORM);
        play(ALARM);
        Serial.print(_p_temp);
        Serial.println("PLATFORM SemiCon Cool max temp triggerred!");
        platform_max_temp = true;
      } else
      {
        platform_max_temp = false;
      }
    }
  } else
  {
    platform_on = check_platform();
    if (platform_on)
    {
      Serial.println("PLATFORM Put On");
      play(DIDI);
    }
  }
}

bool check_extruder()
{
  if (get_temp(EXTRUDER) < -20)
  {
    //extruder_on = false;
    return false;
  } else
  {
    //extruder_on = true;
    return true;
  }
}

bool check_platform()
{
  if (get_temp(PLATFORM) < -20)
  {
    //platform_on = false;
    return false;
  } else
  {
    //platform_on = true;
    return true;
  }
}

bool check_extruder_max_temp()
{
  return extruder_max_temp;
}

bool check_platform_max_temp()
{
  return platform_max_temp;
}
