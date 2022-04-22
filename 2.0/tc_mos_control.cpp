#include "tc_mos_control.h"
#include "tc_pins.h"
#include "tc_message.h"
#include "Marlin.h"
#include "fastio.h"

int object_mode[OBJECT_NUM] = {NONE, NONE};
int object_former_mode[OBJECT_NUM] = {NONE, NONE};


uint8_t p_cool_bit, p_cool_port, *p_cool_out;
uint8_t p_heat_bit, p_heat_port, *p_heat_out;
uint8_t e0_cool_bit, e0_cool_port, *e0_cool_out;
uint8_t e0_heat_bit, e0_heat_port, *e0_heat_out;
uint8_t e1_cool_bit, e1_cool_port, *e1_cool_out;
uint8_t e1_heat_bit, e1_heat_port, *e1_heat_out;

void mos_init()
{
  SET_OUTPUT(COOL_PLATFORM_PIN);
  WRITE(COOL_PLATFORM_PIN, HIGH);

  SET_OUTPUT(HEATER_PLATFORM_PIN);
  WRITE(HEATER_PLATFORM_PIN, HIGH);

  SET_OUTPUT(COOL_CAVITY_E0_PIN);
  WRITE(COOL_CAVITY_E0_PIN, HIGH);

  SET_OUTPUT(HEATER_CAVITY_E0_PIN);
  WRITE(HEATER_CAVITY_E0_PIN, HIGH);

  SET_OUTPUT(COOL_CAVITY_E1_PIN);
  WRITE(COOL_CAVITY_E1_PIN, HIGH);

  SET_OUTPUT(HEATER_CAVITY_E1_PIN);
  WRITE(HEATER_CAVITY_E1_PIN, HIGH);
}

void control(byte _object, int _object_mode, byte _en)
{
  object_mode[_object] = _object_mode;

  switch (_object)
  {
    case PLATFORM:
      {
        if (_object_mode == HEATER)
        {
          WRITE(COOL_PLATFORM_PIN, HIGH);
          if (_en == ENABLE)
            WRITE(HEATER_PLATFORM_PIN, LOW);
          else if (_en == DISABLE)
            WRITE(HEATER_PLATFORM_PIN, HIGH);
        }
        else if (_object_mode == COOL)
        {
          WRITE(HEATER_PLATFORM_PIN, HIGH);
          if (_en == ENABLE)
            WRITE(COOL_PLATFORM_PIN, LOW);
          else if (_en == DISABLE)
            WRITE(COOL_PLATFORM_PIN, HIGH);
        } else if (_object_mode == MEMBRANE)
        {
          WRITE(COOL_PLATFORM_PIN, HIGH);
          if (_en == ENABLE)
            WRITE(HEATER_PLATFORM_PIN, LOW);
          else if (_en == DISABLE)
            WRITE(HEATER_PLATFORM_PIN, HIGH);
        }
        else if (_object_mode == NONE)
        {
          WRITE(COOL_PLATFORM_PIN, HIGH);
          WRITE(HEATER_PLATFORM_PIN, HIGH);
          SERIAL_PROTOCOLLNPGM(MSG_PLATFORM_OFF);
        }
      }
      break;
    case E0_CAVITY:
      {
        if (_object_mode == HEATER)
        {
          WRITE(COOL_CAVITY_E0_PIN, HIGH);
          if (_en == ENABLE)
            WRITE(HEATER_CAVITY_E0_PIN, LOW);
          else if (_en == DISABLE)
            WRITE(HEATER_CAVITY_E0_PIN, HIGH);
        }
        else if (_object_mode == COOL)
        {
          WRITE(HEATER_CAVITY_E0_PIN, HIGH);
          if (_en == ENABLE)
            WRITE(COOL_CAVITY_E0_PIN, LOW);
          else if (_en == DISABLE)
            WRITE(COOL_CAVITY_E0_PIN, HIGH);
        } else if (_object_mode == MEMBRANE)
        {
          WRITE(COOL_CAVITY_E0_PIN, HIGH);
          if (_en == ENABLE)
            WRITE(HEATER_CAVITY_E0_PIN, LOW);
          else if (_en == DISABLE)
            WRITE(HEATER_CAVITY_E0_PIN, HIGH);
        }
        else if (_object_mode == NONE)
        {
          WRITE(COOL_CAVITY_E0_PIN, HIGH);
          WRITE(HEATER_CAVITY_E0_PIN, HIGH);
          SERIAL_PROTOCOLLNPGM(MSG_E0_CAVITY_OFF);
        }
      }
      break;
    case E1_CAVITY:
      {
        if (_object_mode == HEATER)
        {
          WRITE(COOL_CAVITY_E1_PIN, HIGH);
          if (_en == ENABLE)
            WRITE(HEATER_CAVITY_E1_PIN, LOW);
          else if (_en == DISABLE)
            WRITE(HEATER_CAVITY_E1_PIN, HIGH);
        }
        else if (_object_mode == COOL)
        {
          WRITE(HEATER_CAVITY_E1_PIN, HIGH);
          if (_en == ENABLE)
            WRITE(COOL_CAVITY_E1_PIN, LOW);
          else if (_en == DISABLE)
            WRITE(COOL_CAVITY_E1_PIN, HIGH);
        } else if (_object_mode == MEMBRANE)
        {
          WRITE(COOL_CAVITY_E1_PIN, HIGH);
          if (_en == ENABLE)
            WRITE(HEATER_CAVITY_E1_PIN, LOW);
          else if (_en == DISABLE)
            WRITE(HEATER_CAVITY_E1_PIN, HIGH);
        }
        else if (_object_mode == NONE)
        {
          WRITE(COOL_CAVITY_E1_PIN, HIGH);
          WRITE(HEATER_CAVITY_E1_PIN, HIGH);
          SERIAL_PROTOCOLLNPGM(MSG_E1_CAVITY_OFF);
        }
      }
      break;
  }
}

/*
  void mos_init()
  {
    pinMode(COOL_PLATFORM_PIN, OUTPUT);
    p_cool_bit = digitalPinToBitMask(COOL_PLATFORM_PIN);
    p_cool_port = digitalPinToPort(COOL_PLATFORM_PIN);
    p_cool_out = portOutputRegister(p_cool_port);
     p_cool_out |= p_cool_bit;

    pinMode(HEATER_PLATFORM_PIN, OUTPUT);
    p_heat_bit = digitalPinToBitMask(HEATER_PLATFORM_PIN);
    p_heat_port = digitalPinToPort(HEATER_PLATFORM_PIN);
    p_heat_out = portOutputRegister(p_heat_port);
    p_heat_out |= p_heat_bit;


    pinMode(COOL_CAVITY_E0_PIN, OUTPUT);
    e0_cool_bit = digitalPinToBitMask(COOL_CAVITY_E0_PIN);
    e0_cool_port = digitalPinToPort(COOL_CAVITY_E0_PIN);
    e0_cool_out = portOutputRegister(e0_cool_port);
    e0_cool_out |= e0_cool_bit;


  pinMode(HEATER_CAVITY_E0_PIN, OUTPUT);
  e0_heat_bit = digitalPinToBitMask(HEATER_CAVITY_E0_PIN);
  e0_heat_port = digitalPinToPort(HEATER_CAVITY_E0_PIN);
  e0_heat_out = portOutputRegister(e0_heat_port);
   e0_heat_out |= e0_heat_bit;

  pinMode(COOL_CAVITY_E1_PIN, OUTPUT);
  e1_cool_bit = digitalPinToBitMask(COOL_CAVITY_E1_PIN);
  e1_cool_port = digitalPinToPort(COOL_CAVITY_E1_PIN);
  e1_cool_out = portOutputRegister(e1_cool_port);
   e1_cool_out |= e1_cool_bit;

  pinMode(HEATER_CAVITY_E1_PIN, OUTPUT);
  e1_heat_bit = digitalPinToBitMask(HEATER_CAVITY_E1_PIN);
  e1_heat_port = digitalPinToPort(HEATER_CAVITY_E1_PIN);
  e1_heat_out = portOutputRegister(e1_heat_port);
   e1_heat_out |= e1_heat_bit;
  }

  void control(byte _object, int _object_mode, byte _en)
  {
  object_mode[_object] = _object_mode;

  switch (_object)
  {
    case PLATFORM:
      {
        if (_object_mode == HEATER)
        {
           p_cool_out |= p_cool_bit;
          if (_en == ENABLE)
             p_heat_out &= ~p_heat_bit;
          else if (_en == DISABLE)
             p_heat_out |= p_heat_bit;
        }
        else if (_object_mode == COOL)
        {
           p_heat_out |= p_heat_bit;
          if (_en == ENABLE)
             p_cool_out &= ~p_cool_bit;
          else if (_en == DISABLE)
             p_cool_out |= p_cool_bit;
        } else if (_object_mode == MEMBRANE)
        {
           p_cool_out |= p_cool_bit;
          if (_en == ENABLE)
             p_heat_out &= ~p_heat_bit;
          else if (_en == DISABLE)
             p_heat_out |= p_heat_bit;
        }
        else if (_object_mode == NONE)
        {
           p_heat_out |= p_heat_bit;
           p_cool_out |= p_cool_bit;
          SERIAL_PROTOCOLPGM(MSG_PLATFORM_OFF);
        }
      }
      break;
    case E0_CAVITY:
      {
        if (_object_mode == HEATER)
        {
           e0_cool_out |= e0_cool_bit;
          if (_en == ENABLE)
             e0_heat_out &= ~e0_heat_bit;
          else if (_en == DISABLE)
             e0_heat_out |= e0_heat_bit;
        }
        else if (_object_mode == COOL)
        {
           e0_heat_out |= e0_heat_bit;
          if (_en == ENABLE)
             e0_cool_out &= ~e0_cool_bit;
          else if (_en == DISABLE)
             e0_cool_out |= e0_cool_bit;
        } else if (_object_mode == MEMBRANE)
        {
           e0_cool_out |= e0_cool_bit;
          if (_en == ENABLE)
             e0_heat_out &= ~e0_heat_bit;
          else if (_en == DISABLE)
             e0_heat_out |= e0_heat_bit;
        }
        else if (_object_mode == NONE)
        {
           e0_heat_out |= e0_heat_bit;
           e0_cool_out |= e0_cool_bit;
          SERIAL_PROTOCOLPGM(MSG_E0_CAVITY_OFF);
        }
      }
      break;
    case E1_CAVITY:
      {
        if (_object_mode == HEATER)
        {
           e1_cool_out |= e1_cool_bit;
          if (_en == ENABLE)
             e1_heat_out &= ~e1_heat_bit;
          else if (_en == DISABLE)
             e1_heat_out |= e1_heat_bit;
        }
        else if (_object_mode == COOL)
        {
           e1_heat_out |= e1_heat_bit;
          if (_en == ENABLE)
             e1_cool_out &= ~e1_cool_bit;
          else if (_en == DISABLE)
             e1_cool_out |= e1_cool_bit;
        } else if (_object_mode == MEMBRANE)
        {
           e1_cool_out |= e1_cool_bit;
          if (_en == ENABLE)
             e1_heat_out &= ~e1_heat_bit;
          else if (_en == DISABLE)
             e1_heat_out |= e1_heat_bit;
        }
        else if (_object_mode == NONE)
        {
           e1_heat_out |= e1_heat_bit;
           e1_cool_out |= e1_cool_bit;
          SERIAL_PROTOCOLPGM(MSG_E1_CAVITY_OFF);
        }
      }
      break;
  }
  }
*/
