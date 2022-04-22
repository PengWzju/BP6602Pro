#include "temperature.h"
//#include "ultralcd.h"
#include "ConfigurationStore.h"

void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size)
{
  do
  {
    eeprom_write_byte((unsigned char*)pos, *value);
    pos++;
    value++;
  } while (--size);
}
#define EEPROM_WRITE_VAR(pos, value) _EEPROM_writeData(pos, (uint8_t*)&value, sizeof(value))
void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size)
{
  do
  {
    *value = eeprom_read_byte((unsigned char*)pos);
    pos++;
    value++;
  } while (--size);
}
#define EEPROM_READ_VAR(pos, value) _EEPROM_readData(pos, (uint8_t*)&value, sizeof(value))
//======================================================================================




#define EEPROM_OFFSET 0


// IMPORTANT:  Whenever there are changes made to the variables stored in EEPROM
// in the functions below, also increment the version number. This makes sure that
// the default values are used whenever there is a change to the data, to prevent
// wrong data being written to the variables.
// ALSO:  always make sure the variables in the Store and retrieve sections are in the same order.
#define EEPROM_VERSION "V07"

#ifdef EEPROM_SETTINGS
void Config_StoreSettings()
{
  char ver[4] = "000";
  int i = EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i, ver); // invalidate data first
  /*
    EEPROM_WRITE_VAR(i,axis_steps_per_unit);
    EEPROM_WRITE_VAR(i,max_feedrate);
    EEPROM_WRITE_VAR(i,max_acceleration_units_per_sq_second);
    EEPROM_WRITE_VAR(i,acceleration);
    EEPROM_WRITE_VAR(i,retract_acceleration);
    EEPROM_WRITE_VAR(i,minimumfeedrate);
    EEPROM_WRITE_VAR(i,mintravelfeedrate);
    EEPROM_WRITE_VAR(i,minsegmenttime);
    EEPROM_WRITE_VAR(i,max_xy_jerk);
    EEPROM_WRITE_VAR(i,max_z_jerk);
    EEPROM_WRITE_VAR(i,max_e_jerk);
    EEPROM_WRITE_VAR(i,add_homeing);
    #ifndef ULTIPANEL
    int plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP, plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP, plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED;
    int absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP, absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP, absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
    #endif
    EEPROM_WRITE_VAR(i,plaPreheatHotendTemp);
    EEPROM_WRITE_VAR(i,plaPreheatHPBTemp);
    EEPROM_WRITE_VAR(i,plaPreheatFanSpeed);
    EEPROM_WRITE_VAR(i,absPreheatHotendTemp);
    EEPROM_WRITE_VAR(i,absPreheatHPBTemp);
    EEPROM_WRITE_VAR(i,absPreheatFanSpeed);
    #ifdef PIDTEMP
      EEPROM_WRITE_VAR(i,Kp);
      EEPROM_WRITE_VAR(i,Ki);
      EEPROM_WRITE_VAR(i,Kd);
    #else
  		float dummy = 3000.0f;
      EEPROM_WRITE_VAR(i,dummy);
  		dummy = 0.0f;
      EEPROM_WRITE_VAR(i,dummy);
      EEPROM_WRITE_VAR(i,dummy);
    #endif
  */
  // wp add start
  EEPROM_WRITE_VAR(i, ec_kp);
  EEPROM_WRITE_VAR(i, ec_ki);
  EEPROM_WRITE_VAR(i, ec_kd);
  EEPROM_WRITE_VAR(i, eh_kp);
  EEPROM_WRITE_VAR(i, eh_ki);
  EEPROM_WRITE_VAR(i, eh_kd);
  EEPROM_WRITE_VAR(i, e_kp);
  EEPROM_WRITE_VAR(i, e_ki);
  EEPROM_WRITE_VAR(i, e_kd);

  EEPROM_WRITE_VAR(i, pc_kp);
  EEPROM_WRITE_VAR(i, pc_ki);
  EEPROM_WRITE_VAR(i, pc_kd);
  EEPROM_WRITE_VAR(i, ph_kp);
  EEPROM_WRITE_VAR(i, ph_ki);
  EEPROM_WRITE_VAR(i, ph_kd);
  EEPROM_WRITE_VAR(i, p_kp);
  EEPROM_WRITE_VAR(i, p_ki);
  EEPROM_WRITE_VAR(i, p_kd);
  // wp add end
  char ver2[4] = EEPROM_VERSION;
  i = EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i, ver2); // validate data
  //  SERIAL_ECHO_START;
  //  SERIAL_ECHOLNPGM("Settings Stored");
}
#endif //EEPROM_SETTINGS


#ifdef EEPROM_CHITCHAT
void Config_PrintSettings()
{ // Always have this function, even with EEPROM_SETTINGS disabled, the current values will be shown

  Serial.print("ec_kp:");
  Serial.print(ec_kp);
  Serial.print(" ec_ki:");
  Serial.print(ec_ki);
  Serial.print(" ec_kd:");
  Serial.print(ec_kd);
  Serial.println();
  Serial.print("eh_kp:");
  Serial.print(eh_kp);
  Serial.print(" eh_ki:");
  Serial.print(eh_ki);
  Serial.print(" eh_kd:");
  Serial.print(eh_kd);
  Serial.println();
  Serial.print("e_kp:");
  Serial.print(e_kp);
  Serial.print(" e_ki:");
  Serial.print(e_ki);
  Serial.print(" e_kd:");
  Serial.print(e_kd);
  Serial.println();

  Serial.print("pc_kp:");
  Serial.print(pc_kp);
  Serial.print(" pc_ki:");
  Serial.print(pc_ki);
  Serial.print(" pc_kd:");
  Serial.print(pc_kd);
  Serial.println();
  Serial.print("ph_kp:");
  Serial.print(ph_kp);
  Serial.print(" ph_ki:");
  Serial.print(ph_ki);
  Serial.print(" ph_kd:");
  Serial.print(ph_kd);
  Serial.println();
  Serial.print("p_kp:");
  Serial.print(p_kp);
  Serial.print(" p_ki:");
  Serial.print(p_ki);
  Serial.print(" p_kd:");
  Serial.print(p_kd);
  Serial.println();
}
#endif


#ifdef EEPROM_SETTINGS
void Config_RetrieveSettings()
{
  int i = EEPROM_OFFSET;
  char stored_ver[4];
  char ver[4] = EEPROM_VERSION;
  EEPROM_READ_VAR(i, stored_ver); //read stored version
  //SERIAL_ECHOLN("Version: [" << ver << "] Stored version: [" << stored_ver << "]");
  if (strncmp(ver, stored_ver, 3) == 0)
  {
    /*
            // version number match
            EEPROM_READ_VAR(i,axis_steps_per_unit);
            EEPROM_READ_VAR(i,max_feedrate);
            EEPROM_READ_VAR(i,max_acceleration_units_per_sq_second);

            // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
    		reset_acceleration_rates();

            EEPROM_READ_VAR(i,acceleration);
            EEPROM_READ_VAR(i,retract_acceleration);
            EEPROM_READ_VAR(i,minimumfeedrate);
            EEPROM_READ_VAR(i,mintravelfeedrate);
            EEPROM_READ_VAR(i,minsegmenttime);
            EEPROM_READ_VAR(i,max_xy_jerk);
            EEPROM_READ_VAR(i,max_z_jerk);
            EEPROM_READ_VAR(i,max_e_jerk);
            EEPROM_READ_VAR(i,add_homeing);
            #ifndef ULTIPANEL
            int plaPreheatHotendTemp, plaPreheatHPBTemp, plaPreheatFanSpeed;
            int absPreheatHotendTemp, absPreheatHPBTemp, absPreheatFanSpeed;
            #endif
            EEPROM_READ_VAR(i,plaPreheatHotendTemp);
            EEPROM_READ_VAR(i,plaPreheatHPBTemp);
            EEPROM_READ_VAR(i,plaPreheatFanSpeed);
            EEPROM_READ_VAR(i,absPreheatHotendTemp);
            EEPROM_READ_VAR(i,absPreheatHPBTemp);
            EEPROM_READ_VAR(i,absPreheatFanSpeed);
            #ifndef PIDTEMP
            float Kp,Ki,Kd;
            #endif
            // do not need to scale PID values as the values in EEPROM are already scaled
            EEPROM_READ_VAR(i,Kp);
            EEPROM_READ_VAR(i,Ki);
            EEPROM_READ_VAR(i,Kd);
    */
    // wp add start
    EEPROM_READ_VAR(i, ec_kp);
    EEPROM_READ_VAR(i, ec_ki);
    EEPROM_READ_VAR(i, ec_kd);
    EEPROM_READ_VAR(i, eh_kp);
    EEPROM_READ_VAR(i, eh_ki);
    EEPROM_READ_VAR(i, eh_kd);
    EEPROM_READ_VAR(i, e_kp);
    EEPROM_READ_VAR(i, e_ki);
    EEPROM_READ_VAR(i, e_kd);

    EEPROM_READ_VAR(i, pc_kp);
    EEPROM_READ_VAR(i, pc_ki);
    EEPROM_READ_VAR(i, pc_kd);
    EEPROM_READ_VAR(i, ph_kp);
    EEPROM_READ_VAR(i, ph_ki);
    EEPROM_READ_VAR(i, ph_kd);
    EEPROM_READ_VAR(i, p_kp);
    EEPROM_READ_VAR(i, p_ki);
    EEPROM_READ_VAR(i, p_kd);    
    // wp add end

    // Call updatePID (similar to when we have processed M301)
    //		updatePID();
    //        SERIAL_ECHO_START;
    //        SERIAL_ECHOLNPGM("Stored settings retrieved");
  }
  else
  {
    Config_ResetDefault();
  }
  Config_PrintSettings();
}
#endif

void Config_ResetDefault()
{
  /*
      float tmp1[]=DEFAULT_AXIS_STEPS_PER_UNIT;
      float tmp2[]=DEFAULT_MAX_FEEDRATE;
      long tmp3[]=DEFAULT_MAX_ACCELERATION;
      for (short i=0;i<4;i++)
      {
          axis_steps_per_unit[i]=tmp1[i];
          max_feedrate[i]=tmp2[i];
          max_acceleration_units_per_sq_second[i]=tmp3[i];
      }

      // steps per sq second need to be updated to agree with the units per sq second
      reset_acceleration_rates();

      acceleration=DEFAULT_ACCELERATION;
      retract_acceleration=DEFAULT_RETRACT_ACCELERATION;
      minimumfeedrate=DEFAULT_MINIMUMFEEDRATE;
      minsegmenttime=DEFAULT_MINSEGMENTTIME;
      mintravelfeedrate=DEFAULT_MINTRAVELFEEDRATE;
      max_xy_jerk=DEFAULT_XYJERK;
      max_z_jerk=DEFAULT_ZJERK;
      max_e_jerk=DEFAULT_EJERK;
      add_homeing[0] = add_homeing[1] = add_homeing[2] = 0;
    #ifdef ULTIPANEL
      plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP;
      plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP;
      plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED;
      absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP;
      absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP;
      absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
    #endif
    #ifdef PIDTEMP
      Kp = DEFAULT_Kp;
      Ki = scalePID_i(DEFAULT_Ki);
      Kd = scalePID_d(DEFAULT_Kd);

      // call updatePID (similar to when we have processed M301)
      updatePID();

    #ifdef PID_ADD_EXTRUSION_RATE
      Kc = DEFAULT_Kc;
    #endif//PID_ADD_EXTRUSION_RATE
    #endif//PIDTEMP
  */
  // wp add start
  ec_kp = EC_KP;
  ec_ki = EC_KI;
  ec_kd = EC_KD;
  eh_kp = EH_KP;
  eh_ki = EH_KI;
  eh_kd = EH_KD;
  e_kp = E_KP;
  e_ki = E_KI;
  e_kd = E_KD;

  pc_kp = PC_KP;
  pc_ki = PC_KI;
  pc_kd = PC_KD;
  ph_kp = PH_KP;
  ph_ki = PH_KI;
  ph_kd = PH_KD;
  p_kp = P_KP;
  p_ki = P_KI;
  p_kd = P_KD;  
  // wp add end
  //SERIAL_ECHO_START;
  //SERIAL_ECHOLNPGM("Hardcoded Default Settings Loaded");

}
