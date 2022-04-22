#include "tc_self_check.h"
#include "tc_temperature.h"
#include "temperature.h"
#include "tc_pins.h"
#include "tc_buzzer.h"
#include "marlin.h"

// ----------------- private variables ---------------

#define SEMICON_HEAT_ALARM_TEMP 80
#define SEMICON_COOL_ALARM_TEMP 30
#define MEMBRANE_ALARM_TEMP 120
#define PIPE_ALARM_TEMP 270

bool e0_on = false;
bool e1_on = false;
bool platform_on = false;
bool e0_max_temp = false;
bool e1_max_temp = false;
bool platform_max_temp = false;
int alarm_temp[3] = { -1, -1, -1};

// --------------- private functions -------------------
bool check_e0();
bool check_e1();
bool check_platform();
bool check_e0_max_temp();
bool chech_e1_max_temp();
bool e0_has_temp();
bool e1_has_temp();
bool platform_has_temp();
bool check_platform_max_temp();

void check_init()
{
  e0_on = check_e0();
  e1_on = check_e1();
  platform_on = check_platform();
//  if (e0_on)
//    play(DIDI);
//  if (e1_on)
//    play(DIDI);
//  if (platform_on)
//    play(DIDI);
}

void max_temp_check()
{
  if (e0_on)
  {
    e0_on = check_e0();
    double _e_temp = degHotend(0);
    if (!e0_on)
    {
      if (e0_has_temp())
        stop_heat(E0_CAVITY);
      SERIAL_PROTOCOLLNPGM("E0 Takes Off");
      play(ALARM);
    }
    if (e0_max_temp)
    {
      if (_e_temp > alarm_temp[E0_CAVITY])
      {
        play(ALARM);
        SERIAL_PROTOCOL_F(_e_temp, 1);
        SERIAL_PROTOCOLLNPGM("\tE0 CAVITY MAX temp triggered!");
      }
      else
        e0_max_temp = false;
    }
    if (e0_has_temp() && object_mode[E0_CAVITY] == MEMBRANE)
    {
      if (_e_temp > PIPE_ALARM_TEMP)
      {
        stop_heat(E0_CAVITY);
        alarm_temp[E0_CAVITY] = PIPE_ALARM_TEMP;
        play(ALARM);
        SERIAL_PROTOCOL_F(_e_temp, 1);
        SERIAL_PROTOCOLLNPGM("E0 CAVITY PIPE MAX temp triggered!");
        e0_max_temp = true;
      } else
      {
        e0_max_temp = false;
      }
    } else if (e0_has_temp() && (object_mode[E0_CAVITY] == HEATER))
    {
      if (_e_temp > SEMICON_HEAT_ALARM_TEMP)
      {
        stop_heat(E0_CAVITY);
        alarm_temp[E0_CAVITY] = SEMICON_HEAT_ALARM_TEMP;
        play(ALARM);
        SERIAL_PROTOCOL_F(_e_temp, 1);
        SERIAL_PROTOCOLLNPGM("E0 CAVITY SemiCon Heat max temp triggerred!");
        e0_max_temp = true;
      } else
      {
        e0_max_temp = false;
      }
    } else if (e0_has_temp() && (object_mode[E0_CAVITY] == COOL))
    {
      if (_e_temp > SEMICON_COOL_ALARM_TEMP)
      {
        stop_heat(E0_CAVITY);
        alarm_temp[E0_CAVITY] = SEMICON_COOL_ALARM_TEMP;
        play(ALARM);
        SERIAL_PROTOCOL_F(_e_temp, 1);
        SERIAL_PROTOCOLLNPGM("E0 CAVITY Semicon Cool max temp triggerred!");
        e0_max_temp = true;
      } else
      {
        e0_max_temp = false;
      }
    }
  } else
  {
    e0_on = check_e0();
    if (e0_on)
    {
      SERIAL_PROTOCOLLNPGM("E0 Put On");
      play(DIDI);
    }
  }
  if (e1_on)
  {
    e1_on = check_e1();
    double _e_temp = degHotend(1);
    if (!e1_on)
    {
      if (e1_has_temp())
        stop_heat(E1_CAVITY);
      SERIAL_PROTOCOLLNPGM("E1 Takes Off");
      play(ALARM);
    }
    if (e1_max_temp)
    {
      if (_e_temp > alarm_temp[E1_CAVITY])
      {
        play(ALARM);
        SERIAL_PROTOCOL_F(_e_temp, 1);
        SERIAL_PROTOCOLLNPGM("\tE1 CAVITY MAX temp triggered!");
      }
      else
        e1_max_temp = false;
    }
    if (e1_has_temp() && (object_mode[E1_CAVITY] == MEMBRANE))
    {
      if (_e_temp > PIPE_ALARM_TEMP)
      {
        stop_heat(E1_CAVITY);
        alarm_temp[E1_CAVITY] = PIPE_ALARM_TEMP;
        play(ALARM);
        SERIAL_PROTOCOL_F(_e_temp, 1);
        SERIAL_PROTOCOLLNPGM("E1 CAVITY MAX temp triggered!");
        e1_max_temp = true;
      } else
      {
        e1_max_temp = false;
      }
    } else if (e1_has_temp() && (object_mode[E1_CAVITY] == HEATER))
    {
      if (_e_temp > SEMICON_HEAT_ALARM_TEMP)
      {
        stop_heat(E1_CAVITY);
        alarm_temp[E1_CAVITY] = SEMICON_HEAT_ALARM_TEMP;
        play(ALARM);
        SERIAL_PROTOCOL_F(_e_temp, 1);
        SERIAL_PROTOCOLLNPGM("E1 CAVITY SemiCon Heat max temp triggerred!");
        e1_max_temp = true;
      } else
      {
        e1_max_temp = false;
      }
    } else if (e1_has_temp() && (object_mode[E1_CAVITY] == COOL))
    {
      if (_e_temp > SEMICON_COOL_ALARM_TEMP)
      {
        stop_heat(E1_CAVITY);
        alarm_temp[E1_CAVITY] = SEMICON_COOL_ALARM_TEMP;
        play(ALARM);
        SERIAL_PROTOCOL_F(_e_temp, 1);
        SERIAL_PROTOCOLLNPGM("E1 CAVITY Semicon Cool max temp triggerred!");
        e1_max_temp = true;
      } else
      {
        e1_max_temp = false;
      }
    }
  } else
  {
    e1_on = check_e1();
    if (e1_on)
    {
      SERIAL_PROTOCOLLNPGM("E1 Puts On");
      play(DIDI);
    }
  }
  if (platform_on)
  {
    platform_on = check_platform();
    double _p_temp = degBed();
    if (!platform_on)
    {
      if (platform_has_temp())
        stop_heat(PLATFORM);
      SERIAL_PROTOCOLLNPGM("PLATFORM Takes Off");
      play(ALARM);
    }
    if (platform_max_temp)
    {
      if (_p_temp > alarm_temp[PLATFORM])
      {
        play(ALARM);
        SERIAL_PROTOCOL_F(_p_temp, 1);
        SERIAL_PROTOCOLLNPGM("\tPLATFORM max temp triggerred!");
      }
      else
        platform_max_temp = false;
    }
    if (platform_has_temp() && (object_mode[PLATFORM] == MEMBRANE))
    {
      if (_p_temp > MEMBRANE_ALARM_TEMP)
      {
        stop_heat(PLATFORM);
        alarm_temp[PLATFORM] = MEMBRANE_ALARM_TEMP;
        play(ALARM);
        SERIAL_PROTOCOL_F(_p_temp, 1);
        SERIAL_PROTOCOLLNPGM("PLATFORM max temp triggerred!");
        platform_max_temp = true;
      } else
      {
        platform_max_temp = false;
      }
    } else if (platform_has_temp() && (object_mode[PLATFORM] == HEATER))
    {
      if (_p_temp > SEMICON_HEAT_ALARM_TEMP)
      {
        stop_heat(PLATFORM);
        alarm_temp[PLATFORM] = SEMICON_HEAT_ALARM_TEMP;
        play(ALARM);
        SERIAL_PROTOCOL_F(_p_temp, 1);
        SERIAL_PROTOCOLLNPGM("PLATFORM SemiCon max temp triggerred!");
        platform_max_temp = true;
      } else
      {
        platform_max_temp = false;
      }
    } else if (platform_has_temp && (object_mode[PLATFORM] == COOL))
    {
      if (_p_temp > SEMICON_COOL_ALARM_TEMP)
      {
        stop_heat(PLATFORM);
        alarm_temp[PLATFORM] = SEMICON_COOL_ALARM_TEMP;
        play(ALARM);
        SERIAL_PROTOCOL_F(_p_temp, 1);
        SERIAL_PROTOCOLLNPGM("PLATFORM SemiCon Cool max temp triggerred!");
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
      SERIAL_PROTOCOLLNPGM("PLATFORM Puts On");
      play(DIDI);
    }
  }
}

/*
  bool check_e0()
  {
  if (degHotend(0) < -20)
    return false;
  else
    return true;
  }

  bool check_e1()
  {
  if (degHotend(1) < -20)
    return false;
  else
    return true;
  }

  bool check_platform()
  {
  if (degBed() < -20)
    return false;
  else
    return true;
  }
*/

bool check_e0()
{
  if (idExtruder(0) > 950)
    return false;
  else
    return true;
}

bool check_e1()
{
  if (idExtruder(1) > 950)
    return false;
  else
    return true;
}

bool check_platform()
{
  if (idBed() > 950)
    return false;
  else
    return true;
}

bool check_e0_max_temp()
{
  return e0_max_temp;
}

bool check_e1_max_temp()
{
  return e1_max_temp;
}

bool check_platform_max_temp()
{
  return platform_max_temp;
}


bool e0_has_temp()
{
  if (abs(idExtruder(0) - 700) < 150)
    return true;
  if (abs(idExtruder(0) - 350) < 100)
    return true;
  return false;
}

bool e1_has_temp()
{
  if (abs(idExtruder(1) - 700) < 150)
    return true;
  if (abs(idExtruder(1) - 350) < 100)
    return true;
  return false;
}

bool platform_has_temp()
{
  if (abs(idBed() - 750) < 100)
    return true;
  return false;
}
