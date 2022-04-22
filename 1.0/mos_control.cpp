#include "mos_control.h"
#include "pins.h"

#define DELAY_MOS 10

int object_mode[OBJECT_NUM] = {NONE, NONE, NONE};
int object_former_mode[OBJECT_NUM] = {NONE, NONE, NONE};
byte object_enable[OBJECT_NUM];

uint8_t p_cool_bit, p_cool_port, *p_cool_out;
uint8_t p_heat_bit, p_heat_port, *p_heat_out;
uint8_t e_cool_bit, e_cool_port, *e_cool_out;
uint8_t e_heat_bit, e_heat_port, *e_heat_out;
uint8_t fan_bit, fan_port, *fan_out;
uint8_t pump_bit, pump_port, *pump_out;

void mos_init()
{
  pinMode(COOL_PLATFORM_PIN, OUTPUT);
  p_cool_bit = digitalPinToBitMask(COOL_PLATFORM_PIN);
  p_cool_port = digitalPinToPort(COOL_PLATFORM_PIN);
  p_cool_out = portOutputRegister(p_cool_port);
  *p_cool_out |= p_cool_bit;

  pinMode(HEATER_PLATFORM_PIN, OUTPUT);
  p_heat_bit = digitalPinToBitMask(HEATER_PLATFORM_PIN);
  p_heat_port = digitalPinToPort(HEATER_PLATFORM_PIN);
  p_heat_out = portOutputRegister(p_heat_port);
  *p_heat_out |= p_heat_bit;

  pinMode(COOL_EXTRUDER_PIN, OUTPUT);
  e_cool_bit = digitalPinToBitMask(COOL_EXTRUDER_PIN);
  e_cool_port = digitalPinToPort(COOL_EXTRUDER_PIN);
  e_cool_out = portOutputRegister(e_cool_port);
  *e_cool_out |= e_cool_bit;

  pinMode(HEATER_EXTRUDER_PIN, OUTPUT);
  e_heat_bit = digitalPinToBitMask(HEATER_EXTRUDER_PIN);
  e_heat_port = digitalPinToPort(HEATER_EXTRUDER_PIN);
  e_heat_out = portOutputRegister(e_heat_port);
  *e_heat_out |= e_heat_bit;

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, HIGH);
  delay(DELAY_MOS);

  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, HIGH);
  delay(DELAY_MOS);

  pinMode(LIQUID_LEVEL_LOW_PIN, INPUT);
  pinMode(LIQUID_LEVEL_HIGH_PIN, INPUT);
}

void control(byte _object, int _object_mode, byte _en)
{
  object_mode[_object] = _object_mode;
  object_enable[_object] = _en;
  switch (_object)
  {
    case PLATFORM:
      {
        if (_object_mode == HEATER)
        {
          *p_cool_out |= p_cool_bit;
          if (_en == ENABLE)
            *p_heat_out &= ~p_heat_bit;
          else if (_en == DISABLE)
            *p_heat_out |= p_heat_bit;
        }
        else if (_object_mode == COOL)
        {
          *p_heat_out |= p_heat_bit;
          if (_en == ENABLE)
            *p_cool_out &= ~p_cool_bit;
          else if (_en == DISABLE)
            *p_cool_out |= p_cool_bit;
        }
        else if (_object_mode == MEMBRANE)
        {
          *p_heat_out |= p_heat_bit;
          if (_en == ENABLE)
            *p_cool_out &= ~p_cool_bit;
          else if (_en == DISABLE)
            *p_cool_out |= p_cool_bit;
        }
        else if (_object_mode == NONE)
        {
          *p_heat_out |= p_heat_bit;
          *p_cool_out |= p_cool_bit;
          Serial.println("platform off");
        }
      }
      break;
    case EXTRUDER:
      {
        if (_object_mode == HEATER)
        {
          *e_cool_out |= e_cool_bit;
          if (_en == ENABLE)
            *e_heat_out &= ~e_heat_bit;
          else if (_en == DISABLE)
            *e_heat_out |= e_heat_bit;
        }
        else if (_object_mode == COOL)
        {
          *e_heat_out |= e_heat_bit;
          if (_en == ENABLE)
            *e_cool_out &= ~e_cool_bit;
          else if (_en == DISABLE)
            *e_cool_out |= e_cool_bit;
        }
        else if (_object_mode == MEMBRANE)
        {
          *e_heat_out |= e_heat_bit;
          if (_en == ENABLE)
            *e_cool_out &= ~e_cool_bit;
          else if (_en == DISABLE)
            *e_cool_out |= e_cool_bit;
        }
        else if (_object_mode == NONE)
        {
          *e_heat_out |= e_heat_bit;
          *e_cool_out |= e_cool_bit;
          Serial.println("extruder off");
        }
      }
      break;
    case FAN:
      {
        digitalWrite(FAN_PIN, _en);
        digitalWrite(PUMP_PIN, _en);
      }
      break;
  }
}
