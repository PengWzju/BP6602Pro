#include "temperature.h"
#include "Configuration.h"
#include "pins.h"
#include "thermistor_tables.h"
#include "PID.h"
#include "usart.h"

// --------------------- public functions -----------------------


// --------------------- private variables -----------------------
#define NUM_TEMPS 2
#define NUM_HEATERS 2

float ec_kp = EC_KP, ec_ki = EC_KI, ec_kd = EC_KD;
float eh_kp = EH_KP, eh_ki = EH_KI, eh_kd = EH_KD;

float pc_kp = PC_KP, pc_ki = PC_KI, pc_kd = PC_KD;
float ph_kp = PH_KP, ph_ki = PH_KI, ph_kd = PH_KD;

float e_kp = E_KP, e_ki = E_KI, e_kd = E_KD;
float p_kp = P_KP, p_ki = P_KI, p_kd = P_KD;


//
double current_temp[NUM_HEATERS];
double target_temp[NUM_HEATERS] = {-48, -48};

byte temp_pin[NUM_TEMPS] = {TEMP_EXTRUDER_PIN, TEMP_PLATFORM_PIN};
double max_temp[NUM_TEMPS] = {EXTRUDER_MAX_TEMP, PLATFORM_MAX_TEMP};
double min_temp[NUM_TEMPS] = {EXTRUDER_MIN_TEMP, PLATFORM_MIN_TEMP};

bool work_on[NUM_HEATERS] = {false, false};
bool start_sample[NUM_HEATERS] = {false, false};

int pid_timer_count[NUM_HEATERS] = {0, 0};

double pid_output[NUM_HEATERS];
double pid_output_current[NUM_HEATERS];

PID extruder_pid(&current_temp[EXTRUDER], &pid_output[EXTRUDER], &target_temp[EXTRUDER], ec_kp, ec_ki, ec_kd, REVERSE);
PID platform_pid(&current_temp[PLATFORM], &pid_output[PLATFORM], &target_temp[PLATFORM], pc_kp, pc_ki, pc_kd, REVERSE);

// --------------------- private functions ------------------------

double read_thermistor(byte pin);
long sample_temp(byte pin);
bool bed_is_on = true;

void temp_init()
{
  extruder_pid.SetOutputLimits(0, WINDOW_SIZE);
  extruder_pid.SetMode(AUTOMATIC);
  platform_pid.SetOutputLimits(0, WINDOW_SIZE);
  platform_pid.SetMode(AUTOMATIC);
}

unsigned long former_time[2] = {0, 0};

void manage_heater()
{
  for (int i = 0; i < NUM_HEATERS; i++)
  {
    if (work_on[i])
    {
      if (!start_sample[i])
      {
        former_time[i] = millis();
        start_sample[i] = true;
        current_temp[i] = get_temp(i);
        if (i == EXTRUDER)
          extruder_pid.Compute();
        else if (i == PLATFORM)
          platform_pid.Compute();
        pid_timer_count[i] = 0;
        pid_output_current[i] = pid_output[i];
      } else
      {
        if (millis() - former_time[i] < (SAMPLE_TIME / 1000))
        {
          continue;
        } else
        {
          former_time[i] = millis();
        }
        current_temp[i] = get_temp(i);
        if (current_temp[i] < -40)
        {
          work_on[i] = false;
          control(i, object_mode[i], DISABLE);
          return;
        }
        pid_timer_count[i] = pid_timer_count[i] + 1;
        if (i == EXTRUDER)
        {
          extruder_pid.Compute();
        } else if (i == PLATFORM)
        {
          platform_pid.Compute();
        }
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
  switch (_mode)
  {
    case NONE:
      {
        work_on[_object] = false;
        control(_object, _mode, DISABLE);
        object_former_mode[_object] = NONE;
        start_sample[_object] = false;
        pid_output_current[_object] = 0;
        pid_output[_object] = 0;
        target_temp[_object] = -48;
      }
      break;
    case HEATER:
      {
        if (object_former_mode[_object] == COOL)
        {
          Serial.println("Turn off COOL first");
          return;
        }
        pid_output[_object] = 0;
        control(FAN, HEATER, ENABLE);
        work_on[_object] = true;
        if (temp > max_temp[_object])
          temp = max_temp[_object];
        if (temp < 0)
          temp = 0;
        target_temp[_object] = temp;
        object_mode[_object] = _mode;
        object_former_mode[_object] = HEATER;
        if (_object == EXTRUDER)
        {
          extruder_pid.SetControllerDirection(DIRECT);
          extruder_pid.SetTunings(eh_kp, eh_ki, eh_kd);
          extruder_pid.SetMode(AUTOMATIC);
          Serial.print("eh_kp:");
          Serial.print(eh_kp, 3);
          Serial.print(" eh_ki:");
          Serial.print(eh_ki, 3);
          Serial.print(" eh_kd:");
          Serial.println(eh_kd, 3);
        } else if (_object == PLATFORM)
        {
          platform_pid.SetControllerDirection(DIRECT);
          platform_pid.SetTunings(ph_kp, ph_ki, ph_kd);
          platform_pid.SetMode(AUTOMATIC);
          Serial.print("ph_kp:");
          Serial.print(ph_kp, 3);
          Serial.print(" ph_ki:");
          Serial.print(ph_ki, 3);
          Serial.print(" ph_kd:");
          Serial.println(ph_kd, 3);
        }
      }
      break;
    case COOL:
      {
        if (object_former_mode[_object] == HEATER)
        {
          Serial.println("Turn off HEATER first");
          return;
        }
        pid_output[_object] = 0;
        control(FAN, HEATER, ENABLE);
        work_on[_object] = true;
        if (temp < min_temp[_object])
          temp = min_temp[_object];
        if (temp > 30)
          temp = 30;
        target_temp[_object] = temp;
        object_mode[_object] = _mode;
        object_former_mode[_object] = COOL;

        if (_object == EXTRUDER)
        {
          extruder_pid.SetControllerDirection(REVERSE);
          extruder_pid.SetTunings(ec_kp, ec_ki, ec_kd);
          extruder_pid.SetMode(AUTOMATIC);
          Serial.print("ec_kp:");
          Serial.print(ec_kp, 3);
          Serial.print(" ec_ki:");
          Serial.print(ec_ki, 3);
          Serial.print(" ec_kd:");
          Serial.println(ec_kd, 3);
        } else if (_object == PLATFORM)
        {
          platform_pid.SetControllerDirection(REVERSE);
          platform_pid.SetTunings(pc_kp, pc_ki, pc_kd);
          platform_pid.SetMode(AUTOMATIC);
          Serial.print("pc_kp:");
          Serial.print(pc_kp, 3);
          Serial.print(" pc_ki:");
          Serial.print(pc_ki, 3);
          Serial.print(" pc_kd:");
          Serial.println(pc_kd, 3);
        }
      }
      break;
    case MEMBRANE:
      {
        pid_output[_object] = 0;
        control(FAN, HEATER, ENABLE);
        work_on[_object] = true;
        if (temp > 100)
          temp = 100;
        if (temp < 0)
          temp = 0;
        target_temp[_object] = temp;
        object_mode[_object] = MEMBRANE;
        object_former_mode[_object] = MEMBRANE;

        if (_object == EXTRUDER)
        {
          extruder_pid.SetControllerDirection(DIRECT);
          extruder_pid.SetTunings(e_kp, e_ki, e_kd);
          extruder_pid.SetMode(AUTOMATIC);
          Serial.print("e_kp:");
          Serial.print(e_kp, 3);
          Serial.print(" e_ki:");
          Serial.print(e_ki, 3);
          Serial.print(" e_kd:");
          Serial.println(e_kd, 3);
        } else if (_object == PLATFORM)
        {
          platform_pid.SetControllerDirection(DIRECT);
          platform_pid.SetTunings(p_kp, p_ki, p_kd);
          platform_pid.SetMode(AUTOMATIC);
          Serial.print("p_kp:");
          Serial.print(p_kp, 3);
          Serial.print(" p_ki:");
          Serial.print(p_ki, 3);
          Serial.print(" p_kd:");
          Serial.println(p_kd, 3);
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
  return read_thermistor(temp_pin[flag]);
}

double read_thermistor(byte pin)
{
  long raw = sample_temp(pin);
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

  return celsius;
}

long sample_temp(byte pin)
{
  long raw = 0;
  long min_raw = analogRead(pin);
  long max_raw = analogRead(pin);
  long sum = 0;
  // read in a certain number of samples
  for (byte i = 0; i < OVER_SAMPLES + 2; i++)
  {
    raw = analogRead(pin);
    sum += raw;
    if (raw < min_raw)
      min_raw = raw;
    if (raw > max_raw)
      max_raw = raw;
  }
  return sum - min_raw - max_raw;
}

void marlin_report_temperatures()
{
  Serial.print("ok T0:");
  Serial.print(get_temp(EXTRUDER));
  Serial.print(" /");
  Serial.print(get_set_temp(EXTRUDER));
  Serial.print(" @0:");
  Serial.print(pid_output_current[EXTRUDER]);
  Serial.print(" B:");
  Serial.print(get_temp(PLATFORM));
  Serial.print(" /");
  Serial.print(get_set_temp(PLATFORM));
  Serial.print(" @0:");
  Serial.println(pid_output_current[PLATFORM]);
}


void stop_heat(byte _object)
{
  //set_temp(0, _object, NONE);
  work_on[_object] = false;
  control(_object, object_mode[_object], DISABLE);
  control(FAN, 0, DISABLE);
  object_former_mode[_object] = NONE;
  start_sample[_object] = false;
  pid_output_current[_object] = 0;
  pid_output[_object] = 0;
  target_temp[_object] = -48;
}
