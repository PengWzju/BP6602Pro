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
          [20200715] 增加加速度查表 modified by wangpeng
              --[modify planner]
  //
  //        created by wangpeng at 20200413
  //        http://www.efl-tech.com
  //
  //======================================================================
*/

#include "tc_main.h"
#include "configuration.h"
#include "efl_serial.h"
#include "message.h"
#include "temperature.h"
#include "usart.h"
#include "pins.h"
#include "mos_control.h"
#include "tc_self_check.h"
#include "buzzer.h"
#include "ConfigurationStore.h"

void setup()
{
  Serial.begin(BAUDRATE);
  mos_init();
  serial_init();
  temp_init();
  buzzer_init();
  Serial.println("start");
  Serial.println("EFL-BP6602_TC");
  Serial.print("Build time:");
  Serial.println(STRING_VERSION_CONFIG_H);
  Serial.print("Version:");
  Serial.println(STRING_CONFIG_H_VERSION);
  check_init();

  // loads data from EEPROM if available else uses defaults (and resets step acceleration rate)
  Config_RetrieveSettings();  
}

void loop( )
{
  //self_test();
  manage_heater();
  read_serial();
  max_temp_check();
#ifdef USART_CONTROLLER
  update_ui();
#endif
}
