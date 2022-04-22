#include "self_test.h"
#include "mos_control.h"
#include "buzzer.h"

#define WAIT_TIME 5000
void self_test()
{
  control(EXTRUDER, COOL, ENABLE);
  delay(WAIT_TIME);
  control(EXTRUDER, COOL, DISABLE);
  delay(10);
  control(EXTRUDER, HEATER, ENABLE);
  delay(WAIT_TIME);
  control(EXTRUDER, HEATER, DISABLE);
  delay(10);

  control(PLATFORM, COOL, ENABLE);
  delay(WAIT_TIME);
  control(PLATFORM, COOL, DISABLE);
  delay(10);
  control(PLATFORM, HEATER, ENABLE);
  delay(WAIT_TIME);
  control(PLATFORM, HEATER, DISABLE);
  delay(10);

  control(FAN, 0, ENABLE);
  delay(WAIT_TIME);
  control(FAN, 0, DISABLE);
  delay(10);

  play(APCHY);
}
