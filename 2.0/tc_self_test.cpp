#include "tc_self_test.h"
#include "tc_mos_control.h"
#include "tc_buzzer.h"

#define WAIT_TIME 3000
void self_test()
{
  control(E0_CAVITY, COOL, ENABLE);
  delay(WAIT_TIME);
  control(E0_CAVITY, NONE, DISABLE);
  delay(WAIT_TIME);
  control(E0_CAVITY, HEATER, ENABLE);
  delay(WAIT_TIME);
  control(E0_CAVITY, NONE, DISABLE);
  delay(WAIT_TIME);
  
  play(ALARM);

  control(E1_CAVITY, COOL, ENABLE);
  delay(WAIT_TIME);
  control(E1_CAVITY, COOL, DISABLE);
  delay(WAIT_TIME);
  control(E1_CAVITY, HEATER, ENABLE);
  delay(WAIT_TIME);
  control(E1_CAVITY, HEATER, DISABLE);
  delay(WAIT_TIME);
  
  play(ALARM);

  control(PLATFORM, COOL, ENABLE);
  delay(WAIT_TIME);
  control(PLATFORM, COOL, DISABLE);
  delay(WAIT_TIME);  
  control(PLATFORM, HEATER, ENABLE);
  delay(WAIT_TIME);
  control(PLATFORM, HEATER, DISABLE);
  delay(WAIT_TIME);

  play(ALARM);
}
