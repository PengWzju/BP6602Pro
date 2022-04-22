#include "tc_temperature.h"
#include "tc_Configuration.h"
#include "tc_pins.h"
#include "tc_thermistor_tables.h"
#include "tc_PID.h"
#include "marlin.h"
#include "temperature.h"
#include "tc_water_pump.h"

// --------------------- public functions -----------------------


// --------------------- private variables -----------------------
#define NUM_HEATERS 3

float e_cavity_cool_pid[3] = E_CAVITY_COOL_PID;
float e_cavity_heat_pid[3] = E_CAVITY_HEAT_PID;
float e_cavity_pipe_pid[3] = E_CAVITY_PIPE_PID;

//float e0_cavity_cool_pid[3] = E0_CAVITY_COOL_PID;
//float e0_cavity_heat_pid[3] = E0_CAVITY_HEAT_PID;
//float e0_cavity_pipe_pid[3] = E0_CAVITY_PIPE_PID;

//float e1_cavity_cool_pid[3] = E1_CAVITY_COOL_PID;
//float e1_cavity_heat_pid[3] = E1_CAVITY_HEAT_PID;
//float e1_cavity_pipe_pid[3] = E1_CAVITY_PIPE_PID;

float p_cool_pid[3] = P_COOL_PID;
float p_heat_pid[3] = P_HEAT_PID;
float p_pipe_pid[3] = P_PIPE_PID;


// auto report temperatures
int auto_report_temp_count = 0;
int auto_report_temp_interval = 0;

//
double current_temp[NUM_HEATERS] = {0};
double target_temp[NUM_HEATERS] = { -48, -48, -48};


bool work_on[NUM_HEATERS] = {false, false, false};
bool start_sample[NUM_HEATERS] = {false, false, false};

int pid_timer_count[NUM_HEATERS] = {0, 0, 0};

double pid_output[NUM_HEATERS] = {0};
double pid_output_current[NUM_HEATERS] = {0};

PID e0_cavity_pid(&current_temp[E0_CAVITY], &pid_output[E0_CAVITY], &target_temp[E0_CAVITY], e_cavity_cool_pid[P], e_cavity_cool_pid[I], e_cavity_cool_pid[D], REVERSE);
PID e1_cavity_pid(&current_temp[E1_CAVITY], &pid_output[E1_CAVITY], &target_temp[E1_CAVITY], e_cavity_cool_pid[P], e_cavity_cool_pid[I], e_cavity_cool_pid[D], REVERSE);

PID platform_pid(&current_temp[PLATFORM], &pid_output[PLATFORM], &target_temp[PLATFORM], p_cool_pid[P], p_cool_pid[I], p_cool_pid[D], REVERSE);

// --------------------- private functions ------------------------

double read_100k_thermistor(unsigned long raw);


void temp_init()
{
  e0_cavity_pid.SetOutputLimits(0, WINDOW_SIZE);
  e0_cavity_pid.init_output();

  e1_cavity_pid.SetOutputLimits(0, WINDOW_SIZE);
  e1_cavity_pid.init_output();

  platform_pid.SetOutputLimits(0, WINDOW_SIZE);
  platform_pid.init_output();

  pump_init();
}

unsigned long former_time[NUM_HEATERS] = {0, 0};

void tc_manage_heater()
{
  for (int i = 0; i < NUM_HEATERS; i++)
  {
    if (work_on[i])
    {
      if (!start_sample[i])
      {
        former_time[i] = millis();
        start_sample[i] = true;
        if (i == E0_CAVITY)
          current_temp[i] = degHotend(0);
        else if (i == E1_CAVITY)
          current_temp[i] = degHotend(1);
        else if (i == PLATFORM)
          current_temp[i] = degBed();
        if (i == E0_CAVITY)
          e0_cavity_pid.Compute();
        else if (i == E1_CAVITY)
          e1_cavity_pid.Compute();
        else if (i == PLATFORM)
          platform_pid.Compute();
        pid_timer_count[i] = 0;
        pid_output_current[i] = pid_output[i];
      } else
      {
        if (millis() - former_time[i] < SAMPLE_TIME / 1000)
        {
          continue;
        } else
        {
          former_time[i] = millis();
        }
        if (i == E0_CAVITY)
          current_temp[i] = degHotend(0);
        else if (i == E1_CAVITY)
          current_temp[i] = degHotend(1);
        else if (i == PLATFORM)
          current_temp[i] = degBed();
        if (current_temp[i] < -40)
        {
          work_on[i] = false;
          control(i, object_mode[i], DISABLE);
          return;
        }
        pid_timer_count[i] = pid_timer_count[i] + 1;
        if (i == E0_CAVITY)
          e0_cavity_pid.Compute();
        else if (i == E1_CAVITY)
          e1_cavity_pid.Compute();
        else if (i == PLATFORM)
          platform_pid.Compute();
        if (pid_timer_count[i] >= WINDOW_SIZE)
        {
          pid_timer_count[i] = 0;
          pid_output_current[i] = pid_output[i];
        }
      }
      if (pid_output_current[i] > pid_timer_count[i])
      {
        control(i, object_mode[i], ENABLE);
      } else
      {
        control(i, object_mode[i], DISABLE);
      }
    }
  }
}

void set_temp(double temp, int _object, int _mode)
{
  switch (_object)
  {
    case E0_CAVITY:
      {
        if (_mode == COOL)
        {
//          if (get_pump_status() != 206)
//          {
//            SERIAL_PROTOCOLLNPGM("ok open pump first!");
//            return;
//          }
          if (object_mode[_object] == HEATER)// || object_mode[_object] != COOL)
          {
            SERIAL_PROTOCOLLNPGM("Turn off heat first!");
            return;
          }
          if (temp < SEMICON_COOL_MIN_TEMP)
            temp = SEMICON_COOL_MIN_TEMP;
          if (temp > 30)
            temp = 30;

          target_temp[_object] = temp;
          object_mode[_object] = _mode;
          work_on[_object] = true;

          e0_cavity_pid.SetControllerDirection(REVERSE);
          e0_cavity_pid.SetTunings(e_cavity_cool_pid[P], e_cavity_cool_pid[I], e_cavity_cool_pid[D]);

          print_pid(_object, _mode);
        } else if (_mode == HEATER)
        {
//          if (get_pump_status() != 206)
//          {
//            SERIAL_PROTOCOLLNPGM("ok open pump first!");
//            return;
//          }          
          if ((object_mode[_object] == COOL))
          {
            SERIAL_PROTOCOLLNPGM("Turn off cool first!");
            return;
          }
          if (temp > SEMICON_HEAT_MAX_TEMP)
            temp = SEMICON_HEAT_MAX_TEMP;
          if (temp < 0)
            temp = 0;

          target_temp[_object] = temp;
          object_mode[_object] = _mode;
          work_on[_object] = true;

          e0_cavity_pid.SetControllerDirection(DIRECT);
          e0_cavity_pid.SetTunings(e_cavity_heat_pid[P], e_cavity_heat_pid[I], e_cavity_heat_pid[D]);

          print_pid(_object, _mode);
        } else if (_mode == MEMBRANE)
        {
          if (temp > PIPE_HEAT_MAX_TEMP)
            temp = PIPE_HEAT_MAX_TEMP;
          if (temp < 0)
            temp = 0;

          target_temp[_object] = temp;
          object_mode[_object] = _mode;
          work_on[_object] = true;

          e0_cavity_pid.SetControllerDirection(DIRECT);
          e0_cavity_pid.SetTunings(e_cavity_pipe_pid[P], e_cavity_pipe_pid[I], e_cavity_pipe_pid[D]);

          print_pid(_object, _mode);
        } else if (_mode == NONE)
        {
          work_on[_object] = false;
          control(_object, _mode, DISABLE);
          target_temp[_object] = -48;
          object_mode[_object] = NONE;
          pid_output[_object] = 0;
          pid_output_current[_object] = 0;
          start_sample[_object] = false;
        }
      }
      break;
    case E1_CAVITY:
      {
        if (_mode == COOL)
        {
//          if (get_pump_status() != 206)
//          {
//            SERIAL_PROTOCOLLNPGM("ok open pump first!");
//            return;
//          }          
          if (object_mode[_object] == HEATER)// || object_mode[_object] != COOL)
          {
            SERIAL_PROTOCOLLNPGM("Turn off heat first!");
            return;
          }
          if (temp < SEMICON_COOL_MIN_TEMP)
            temp = SEMICON_COOL_MIN_TEMP;
          if (temp > 30)
            temp = 30;

          target_temp[_object] = temp;
          object_mode[_object] = _mode;
          work_on[_object] = true;

          e1_cavity_pid.SetControllerDirection(REVERSE);
          e1_cavity_pid.SetTunings(e_cavity_cool_pid[P], e_cavity_cool_pid[I], e_cavity_cool_pid[D]);

          print_pid(_object, _mode);
        } else if (_mode == HEATER)
        {
//          if (get_pump_status() != 206)
//          {
//            SERIAL_PROTOCOLLNPGM("ok open pump first!");
//            return;
//          }          
          if ((object_mode[_object] == COOL))
          {
            SERIAL_PROTOCOLLNPGM("Turn off cool first!");
            return;
          }
          if (temp > SEMICON_HEAT_MAX_TEMP)
            temp = SEMICON_HEAT_MAX_TEMP;
          if (temp < 0)
            temp = 0;

          target_temp[_object] = temp;
          object_mode[_object] = _mode;
          work_on[_object] = true;

          e1_cavity_pid.SetControllerDirection(DIRECT);
          e1_cavity_pid.SetTunings(e_cavity_heat_pid[P], e_cavity_heat_pid[I], e_cavity_heat_pid[D]);

          print_pid(_object, _mode);
        } else if (_mode == MEMBRANE)
        {
          if (temp > PIPE_HEAT_MAX_TEMP)
            temp = PIPE_HEAT_MAX_TEMP;
          if (temp < 0)
            temp = 0;

          target_temp[_object] = temp;
          object_mode[_object] = _mode;
          work_on[_object] = true;

          e1_cavity_pid.SetControllerDirection(DIRECT);
          e1_cavity_pid.SetTunings(e_cavity_pipe_pid[P], e_cavity_pipe_pid[I], e_cavity_pipe_pid[D]);

          print_pid(_object, _mode);
        } else if (_mode == NONE)
        {
          work_on[_object] = false;
          control(_object, _mode, DISABLE);
          target_temp[_object] = -48;
          object_mode[_object] = NONE;
          pid_output[_object] = 0;
          pid_output_current[_object] = 0;
          start_sample[_object] = false;
        }
      }
      break;
    case PLATFORM:
      {
        if (_mode == COOL)
        {
//          if (get_pump_status() != 206)
//          {
//            SERIAL_PROTOCOLLNPGM("ok open pump first!");
//            return;
//          }          
          if (object_mode[_object] == HEATER)
          {
            SERIAL_PROTOCOLLNPGM("Turn off heater first!");
            return;
          }
          if (temp < SEMICON_COOL_MIN_TEMP)
            temp = SEMICON_COOL_MIN_TEMP;
          if (temp > 30)
            temp = 30;

          target_temp[_object] = temp;
          object_mode[_object] = _mode;
          work_on[_object] = true;

          platform_pid.SetControllerDirection(REVERSE);
          platform_pid.SetTunings(p_cool_pid[P], p_cool_pid[I], p_cool_pid[D]);
          print_pid(_object, _mode);
        } else if (_mode == HEATER)
        {
//          if (get_pump_status() != 206)
//          {
//            SERIAL_PROTOCOLLNPGM("ok open pump first!");
//            return;
//          }          
          if (object_mode[_object] == COOL)
          {
            SERIAL_PROTOCOLLNPGM("Turn off cool first!");
            return;
          }
          if (temp > SEMICON_HEAT_MAX_TEMP)
            temp = SEMICON_HEAT_MAX_TEMP;
          if (temp < 0)
            temp = 0;

          target_temp[_object] = temp;
          object_mode[_object] = _mode;
          work_on[_object] = true;

          platform_pid.SetControllerDirection(DIRECT);
          platform_pid.SetTunings(p_heat_pid[P], p_heat_pid[I], p_heat_pid[D]);
          print_pid(_object, _mode);
        } else if (_mode == MEMBRANE)
        {
          if (temp > MEMBRANE_HEAT_MAX_TEMP)
            temp = MEMBRANE_HEAT_MAX_TEMP;
          if (temp < 0)
            temp = 0;

          target_temp[_object] = temp;
          object_mode[_object] = _mode;
          work_on[_object] = true;

          platform_pid.SetControllerDirection(DIRECT);
          platform_pid.SetTunings(p_pipe_pid[P], p_pipe_pid[I], p_pipe_pid[I]);
          print_pid(_object, _mode);
        } else if (_mode == NONE)
        {
          work_on[_object] = false;
          control(_object, _mode, DISABLE);
          target_temp[_object] = -48;
          object_mode[_object] = NONE;
          pid_output[_object] = 0;
          pid_output_current[_object] = 0;
          start_sample[_object] = false;
        }
      }
      break;
  }
}

double get_set_temp(byte _object)
{
  return target_temp[_object];
}

double get_temp(byte flag)
{
  return read_100k_thermistor(flag);
}

double read_100k_thermistor(unsigned long raw)
{
  double celsius = -15;

  float value1;

  byte i = 1;
  for (; i < TEMP_TABLE_SIZE; i++)
  {
    value1 = pgm_read_float_near((int)(&temptable[i][0]));
    if (raw >= value1)
    {
      float value0 = pgm_read_float_near((int)(&temptable[i - 1][0]));
      float temp0 = pgm_read_float_near((int)(&temptable[i - 1][1]));
      float temp1 = pgm_read_float_near((int)(&temptable[i][1]));
      celsius = temp0 + (raw - value0) * (temp1 - temp0) / (value1 - value0);
      break;
    }
  }
  if (i == TEMP_TABLE_SIZE)
  {
    celsius = -50;
  }

  if (i == TEMP_TABLE_SIZE)
  {
    celsius = -50;
  }

  return celsius;
}

void report_identify()
{
  SERIAL_PROTOCOLPGM("ok id T0:"); // T_M -> T0
  SERIAL_PROTOCOL_F(degHotend(0), 1);
  SERIAL_PROTOCOLPGM(" /");
  //  SERIAL_PROTOCOL_F(degHotend(0), 1);
  SERIAL_PROTOCOL_F(target_temp[E0_CAVITY], 1)

  SERIAL_PROTOCOLPGM(" T1:");  // T_E -> T1
  SERIAL_PROTOCOL_F(degHotend(1), 1);
  SERIAL_PROTOCOLPGM(" /");
  //  SERIAL_PROTOCOL_F(degHotend(1), 1);
  SERIAL_PROTOCOL_F(target_temp[E1_CAVITY], 1)

  SERIAL_PROTOCOLPGM(" B:");
  SERIAL_PROTOCOL_F(degBed(), 1);
  SERIAL_PROTOCOLPGM(" /");
  //  SERIAL_PROTOCOL_F(degBed(), 1);
  SERIAL_PROTOCOL_F(target_temp[PLATFORM], 1)


  SERIAL_PROTOCOLPGM(" @T0:");
  SERIAL_PROTOCOL(pid_output_current[E0_CAVITY]);

  SERIAL_PROTOCOLPGM(" @T1:");
  SERIAL_PROTOCOL(pid_output_current[E1_CAVITY]);

  SERIAL_PROTOCOLPGM(" @B:");
  SERIAL_PROTOCOL(pid_output_current[PLATFORM]);

  SERIAL_PROTOCOLPGM(" T0id:");
  SERIAL_PROTOCOL(idExtruder(0));

  SERIAL_PROTOCOLPGM(" Bid:");
  SERIAL_PROTOCOL(idBed());

  SERIAL_PROTOCOLPGM(" T1id:");
  SERIAL_PROTOCOL(idExtruder(1));

  SERIAL_PROTOCOLPGM(" PS:");
  MYSERIAL.print(pump_status, DEC);
  SERIAL_PROTOCOLPGM(" PE:");
  MYSERIAL.print(pump_err, DEC);
  SERIAL_PROTOCOLPGM(" PT:");
  SERIAL_PROTOCOL_F(pump_temp, 2);

  SERIAL_PROTOCOLLN("");

//  SERIAL_PROTOCOLPGM(" ok T0:");
//  MYSERIAL.print(get_pump_status(), DEC);
//  SERIAL_PROTOCOLPGM(" T1:");
//  MYSERIAL.print(get_pump_err(), DEC);
//  SERIAL_PROTOCOLPGM(" B:");
//  SERIAL_PROTOCOL_F(get_pump_temp(), 2);
//
//  SERIAL_PROTOCOLLN("");  
}

void report_temperature()
{
  //  SERIAL_PROTOCOLPGM("ok T1:");
  //  SERIAL_PROTOCOL_F(current_temperature[0], 1);
  //  SERIAL_PROTOCOLPGM(" /");
  //  SERIAL_PROTOCOL_F(target_temp[E0_EXTRUDER], 1);
  //  SERIAL_PROTOCOLPGM(" @E:");
  //  SERIAL_PROTOCOL_F(pid_output_current[E0_EXTRUDER], 1);
  //
  //  SERIAL_PROTOCOLPGM(" T0:");
  //  SERIAL_PROTOCOL_F(degHotend(0), 1);
  //  SERIAL_PROTOCOLPGM(" /");
  //  SERIAL_PROTOCOL_F(target_temp[E0_CAVITY], 1);
  //  SERIAL_PROTOCOLPGM(" @C:");
  //  SERIAL_PROTOCOL_F(pid_output_current[E0_CAVITY], 1);
  //
  //  SERIAL_PROTOCOLPGM(" B:");
  //  SERIAL_PROTOCOL_F(degBed(), 1);
  //  SERIAL_PROTOCOLPGM(" /");
  //  SERIAL_PROTOCOL_F(degTargetBed(), 1);
  //  SERIAL_PROTOCOLPGM(" @B:");
  //  SERIAL_PROTOCOL_F(pid_output_current[PLATFORM], 1);
  //
  //  SERIAL_PROTOCOLLN("");
}

void marlin_report_temperatures()
{
  //  get_e0_type();
  //  get_platform_type();
  //  bool _has_object = false;
  //  if (object_type[E0_ID] != EMPTY || object_type[PLATFORM_ID] != EMPTY)
  //    _has_object = true;
  //  if (!_has_object)
  //    return;
  //
  //  if (object_type[E0_ID] == NPDT || object_type[E0_ID] == NMDT || object_type[E0_ID] == NMHT)
  //  {
  //    Serial.print(" T0_M:");
  //    double _m_temp = get_temp(E0_CAVITY);
  //    if (_m_temp < -30)
  //      Serial.print('-');
  //    else
  //      Serial.print(_m_temp);
  //    Serial.print(" /");
  //    Serial.print(get_set_temp(E0_CAVITY));
  //    Serial.print(' ');
  //    Serial.print(pid_output_current[E0_CAVITY]);
  //    Serial.print('@');
  //    Serial.print(WINDOW_SIZE);
  //  } else if (object_type[E0_ID] == NPHT || object_type[E0_ID] == NPHV)
  //  {
  //    Serial.print("ok T0_M:");
  //    double _m_temp = get_temp(E0_CAVITY);
  //    double _e_temp = get_temp(E0_EXTRUDER);
  //    if (_m_temp < -30)
  //      Serial.print('-');
  //    else
  //      Serial.print(_m_temp);
  //    Serial.print(" /");
  //    Serial.print(get_set_temp(E0_CAVITY));
  //    Serial.print(' ');
  //    Serial.print(pid_output_current[E0_CAVITY]);
  //    Serial.print('@');
  //    Serial.print(WINDOW_SIZE);
  //
  //    Serial.print(" T0_E:");
  //    if (_e_temp < -30)
  //      Serial.print('-');
  //    else
  //      Serial.print(_e_temp);
  //    Serial.print(" /");
  //    Serial.print(get_set_temp(E0_EXTRUDER));
  //    Serial.print(' ');
  //    Serial.print(pid_output_current[E0_EXTRUDER]);
  //    Serial.print('@');
  //    Serial.print(WINDOW_SIZE);
  //  }
  //
  //  if (object_type[PLATFORM_ID] == PPDT || object_type[PLATFORM_ID] == PPHT)
  //  {
  //    Serial.print(" B:");
  //    double _bed_temp = get_temp(PLATFORM);
  //    if (_bed_temp < -30)
  //    {
  //      Serial.print('-');
  //    } else
  //    {
  //      Serial.print(_bed_temp);
  //    }
  //    Serial.print(" /");
  //    Serial.print(get_set_temp(PLATFORM));
  //    Serial.print(' ');
  //    Serial.print(pid_output_current[PLATFORM]);
  //    Serial.print('@');
  //    Serial.print(WINDOW_SIZE);
  //  }
  //  Serial.println();
}

void auto_report_temperatures()
{
#ifdef AUTO_REPORT_TEMPERATURES
  if (auto_report_temp_interval <= 0 )
  {
    return;
  }
  if (auto_report_temp_count / 10 >= auto_report_temp_interval)
  {
    marlin_report_temperatures();
    auto_report_temp_count = 0;
  }
#endif
}

void set_auto_report_temp_interval(int _interval)
{
  auto_report_temp_interval = _interval;
}

void stop_heat(byte _object)
{
  set_temp(0, _object, NONE);
}

void stop_all_heat()
{
  set_temp(0, E0_CAVITY, NONE);
  set_temp(0, PLATFORM, NONE);
}

void print_pid(byte _object, byte _mode)
{
  switch (_object)
  {
    case E0_CAVITY:
      {
        if (_mode == HEATER)
        {
          SERIAL_PROTOCOLPGM("e_cavity_heat_kp:");
          SERIAL_PROTOCOL_F(e_cavity_heat_pid[P], 3);
          SERIAL_PROTOCOLPGM(" e_cavity_heat_ki:");
          SERIAL_PROTOCOL_F(e_cavity_heat_pid[I], 3);
          SERIAL_PROTOCOLPGM(" e_cavity_heat_kd:");
          SERIAL_PROTOCOL_F(e_cavity_heat_pid[D], 3);
          SERIAL_PROTOCOLLN("");
        } else if (_mode == COOL)
        {
          SERIAL_PROTOCOLPGM("e_cavity_cool_kp:");
          SERIAL_PROTOCOL_F(e_cavity_cool_pid[P], 3);
          SERIAL_PROTOCOLPGM(" e_cavity_cool_ki:");
          SERIAL_PROTOCOL_F(e_cavity_cool_pid[I], 3);
          SERIAL_PROTOCOLPGM(" e_cavity_cool_kd:");
          SERIAL_PROTOCOL_F(e_cavity_cool_pid[D], 3);
          SERIAL_PROTOCOLLN("");
        } else if (_mode == MEMBRANE)
        {
          SERIAL_PROTOCOLPGM("e_cavity_pipe_kp:");
          SERIAL_PROTOCOL_F(e_cavity_pipe_pid[P], 3);
          SERIAL_PROTOCOLPGM(" e_cavity_pipe_ki:");
          SERIAL_PROTOCOL_F(e_cavity_pipe_pid[I], 3);
          SERIAL_PROTOCOLPGM(" e_cavity_pipe_kd:");
          SERIAL_PROTOCOL_F(e_cavity_pipe_pid[D], 3);
          SERIAL_PROTOCOLLN("");
        }
      }
      break;
    /*
        case E0_CAVITY:
          {
            if (_mode == HEATER)
            {
              SERIAL_PROTOCOLPGM("e0_cavity_heat_kp:");
              SERIAL_PROTOCOL_F(e0_cavity_heat_pid[P], 3);
              SERIAL_PROTOCOLPGM(" e0_cavity_heat_ki:");
              SERIAL_PROTOCOL_F(e0_cavity_heat_pid[I], 3);
              SERIAL_PROTOCOLPGM(" e0_cavity_heat_kd:");
              SERIAL_PROTOCOL_F(e0_cavity_heat_pid[D], 3);
              SERIAL_PROTOCOLLN("");
            } else if (_mode == COOL)
            {
              SERIAL_PROTOCOLPGM("e0_cavity_cool_kp:");
              SERIAL_PROTOCOL_F(e0_cavity_cool_pid[P], 3);
              SERIAL_PROTOCOLPGM(" e0_cavity_cool_ki:");
              SERIAL_PROTOCOL_F(e0_cavity_cool_pid[I], 3);
              SERIAL_PROTOCOLPGM(" e0_cavity_cool_kd:");
              SERIAL_PROTOCOL_F(e0_cavity_cool_pid[D], 3);
              SERIAL_PROTOCOLLN("");
            } else if (_mode == MEMBRANE)
            {
              SERIAL_PROTOCOLPGM("e0_cavity_pipe_kp:");
              SERIAL_PROTOCOL_F(e0_cavity_pipe_pid[P], 3);
              SERIAL_PROTOCOLPGM(" e0_cavity_pipe_ki:");
              SERIAL_PROTOCOL_F(e0_cavity_pipe_pid[I], 3);
              SERIAL_PROTOCOLPGM(" e0_cavity_pipe_kd:");
              SERIAL_PROTOCOL_F(e0_cavity_pipe_pid[D], 3);
              SERIAL_PROTOCOLLN("");
            }
          }
          break;
        case E1_CAVITY:
          {
            if (_mode == HEATER)
            {
              SERIAL_PROTOCOLPGM("e1_cavity_heat_kp:");
              SERIAL_PROTOCOL_F(e1_cavity_heat_pid[P], 3);
              SERIAL_PROTOCOLPGM(" e1_cavity_heat_ki:");
              SERIAL_PROTOCOL_F(e1_cavity_heat_pid[I], 3);
              SERIAL_PROTOCOLPGM(" e1_cavity_heat_kd:");
              SERIAL_PROTOCOL_F(e1_cavity_heat_pid[D], 3);
              SERIAL_PROTOCOLLN("");
            } else if (_mode == COOL)
            {
              SERIAL_PROTOCOLPGM("e1_cavity_cool_kp:");
              SERIAL_PROTOCOL_F(e1_cavity_cool_pid[P], 3);
              SERIAL_PROTOCOLPGM(" e1_cavity_cool_ki:");
              SERIAL_PROTOCOL_F(e1_cavity_cool_pid[I], 3);
              SERIAL_PROTOCOLPGM(" e1_cavity_cool_kd:");
              SERIAL_PROTOCOL_F(e1_cavity_cool_pid[D], 3);
              SERIAL_PROTOCOLLN("");
            } else if (_mode == MEMBRANE)
            {
              SERIAL_PROTOCOLPGM("e1_cavity_pipe_kp:");
              SERIAL_PROTOCOL_F(e1_cavity_pipe_pid[P], 3);
              SERIAL_PROTOCOLPGM(" e1_cavity_pipe_ki:");
              SERIAL_PROTOCOL_F(e1_cavity_pipe_pid[I], 3);
              SERIAL_PROTOCOLPGM(" e1_cavity_pipe_kd:");
              SERIAL_PROTOCOL_F(e1_cavity_pipe_pid[D], 3);
              SERIAL_PROTOCOLLN("");
            }
          }
          break;
    */
    case PLATFORM:
      {
        if (_mode == HEATER)
        {
          SERIAL_PROTOCOLPGM("p_heat_kp:");
          SERIAL_PROTOCOL_F(p_heat_pid[P], 3);
          SERIAL_PROTOCOLPGM("p_heat_ki:");
          SERIAL_PROTOCOL_F(p_heat_pid[I], 3);
          SERIAL_PROTOCOLPGM("p_heat_kd:");
          SERIAL_PROTOCOL_F(p_heat_pid[D], 3);
          SERIAL_PROTOCOLLN("");
        } else if (_mode == COOL)
        {
          SERIAL_PROTOCOLPGM("p_cool_kp:");
          SERIAL_PROTOCOL_F(p_cool_pid[P], 3);
          SERIAL_PROTOCOLPGM("p_cool_ki:");
          SERIAL_PROTOCOL_F(p_cool_pid[I], 3);
          SERIAL_PROTOCOLPGM("p_cool_kd:");
          SERIAL_PROTOCOL_F(p_cool_pid[D], 3);
          SERIAL_PROTOCOLLN("");
        } else if (_mode == MEMBRANE)
        {
          SERIAL_PROTOCOLPGM("p_pipe_kp:");
          SERIAL_PROTOCOL_F(p_pipe_pid[P], 3);
          SERIAL_PROTOCOLPGM("p_pipe_ki:");
          SERIAL_PROTOCOL_F(p_pipe_pid[I], 3);
          SERIAL_PROTOCOLPGM("p_pipe_kd:");
          SERIAL_PROTOCOL_F(p_pipe_pid[D], 3);
          SERIAL_PROTOCOLLN("");
        }
      }
      break;
  }
}
