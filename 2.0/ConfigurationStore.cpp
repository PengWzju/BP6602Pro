#include "Marlin.h"
#include "planner.h"
#include "temperature.h"
//#include "ultralcd.h"
#include "ConfigurationStore.h"
#include "tc_temperature.h"
#include "tc_configuration.h"

void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size)
{
    do
    {
        eeprom_write_byte((unsigned char*)pos, *value);
        pos++;
        value++;
    }while(--size);
}
#define EEPROM_WRITE_VAR(pos, value) _EEPROM_writeData(pos, (uint8_t*)&value, sizeof(value))
void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size)
{
    do
    {
        *value = eeprom_read_byte((unsigned char*)pos);
        pos++;
        value++;
    }while(--size);
}
#define EEPROM_READ_VAR(pos, value) _EEPROM_readData(pos, (uint8_t*)&value, sizeof(value))
//======================================================================================




#define EEPROM_OFFSET 100


// IMPORTANT:  Whenever there are changes made to the variables stored in EEPROM
// in the functions below, also increment the version number. This makes sure that
// the default values are used whenever there is a change to the data, to prevent
// wrong data being written to the variables.
// ALSO:  always make sure the variables in the Store and retrieve sections are in the same order.
#define EEPROM_VERSION "V07"

#ifdef EEPROM_SETTINGS
void Config_StoreSettings() 
{
  char ver[4]= "000";
  int i=EEPROM_OFFSET; 
  EEPROM_WRITE_VAR(i,ver); // invalidate data first 
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

  EEPROM_WRITE_VAR(i,e_cavity_cool_pid[P]);
  EEPROM_WRITE_VAR(i,e_cavity_cool_pid[I]);
  EEPROM_WRITE_VAR(i,e_cavity_cool_pid[D]);
  EEPROM_WRITE_VAR(i,e_cavity_heat_pid[P]);
  EEPROM_WRITE_VAR(i,e_cavity_heat_pid[I]);
  EEPROM_WRITE_VAR(i,e_cavity_heat_pid[D]);
  EEPROM_WRITE_VAR(i,e_cavity_pipe_pid[P]);
  EEPROM_WRITE_VAR(i,e_cavity_pipe_pid[I]);
  EEPROM_WRITE_VAR(i,e_cavity_pipe_pid[D]);  
/*  
  EEPROM_WRITE_VAR(i,e0_cavity_cool_pid[P]);
  EEPROM_WRITE_VAR(i,e0_cavity_cool_pid[I]);
  EEPROM_WRITE_VAR(i,e0_cavity_cool_pid[D]);
  EEPROM_WRITE_VAR(i,e0_cavity_heat_pid[P]);
  EEPROM_WRITE_VAR(i,e0_cavity_heat_pid[I]);
  EEPROM_WRITE_VAR(i,e0_cavity_heat_pid[D]);
  EEPROM_WRITE_VAR(i,e0_cavity_pipe_pid[P]);
  EEPROM_WRITE_VAR(i,e0_cavity_pipe_pid[I]);
  EEPROM_WRITE_VAR(i,e0_cavity_pipe_pid[D]);  

  EEPROM_WRITE_VAR(i,e1_cavity_cool_pid[P]);
  EEPROM_WRITE_VAR(i,e1_cavity_cool_pid[I]);
  EEPROM_WRITE_VAR(i,e1_cavity_cool_pid[D]);
  EEPROM_WRITE_VAR(i,e1_cavity_heat_pid[P]);
  EEPROM_WRITE_VAR(i,e1_cavity_heat_pid[I]);
  EEPROM_WRITE_VAR(i,e1_cavity_heat_pid[D]);
  EEPROM_WRITE_VAR(i,e1_cavity_pipe_pid[P]);
  EEPROM_WRITE_VAR(i,e1_cavity_pipe_pid[I]);
  EEPROM_WRITE_VAR(i,e1_cavity_pipe_pid[D]); 
*/
  EEPROM_WRITE_VAR(i, p_cool_pid[P]);
  EEPROM_WRITE_VAR(i, p_cool_pid[I]);
  EEPROM_WRITE_VAR(i, p_cool_pid[D]);
  EEPROM_WRITE_VAR(i, p_heat_pid[P]);
  EEPROM_WRITE_VAR(i, p_heat_pid[I]);
  EEPROM_WRITE_VAR(i, p_heat_pid[D]);
  EEPROM_WRITE_VAR(i, p_pipe_pid[P]);
  EEPROM_WRITE_VAR(i, p_pipe_pid[I]);
  EEPROM_WRITE_VAR(i, p_pipe_pid[D]);
  
  
  // wp add end
  char ver2[4]=EEPROM_VERSION;
  i=EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i,ver2); // validate data
  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM("Settings Stored");
}
#endif //EEPROM_SETTINGS


#ifdef EEPROM_CHITCHAT
void Config_PrintSettings()
{  // Always have this function, even with EEPROM_SETTINGS disabled, the current values will be shown
/*  
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Steps per unit:");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M92 X",axis_steps_per_unit[0]);
    SERIAL_ECHOPAIR(" Y",axis_steps_per_unit[1]);
    SERIAL_ECHOPAIR(" Z",axis_steps_per_unit[2]);
    SERIAL_ECHOPAIR(" E",axis_steps_per_unit[3]);
    SERIAL_ECHOLN("");
      
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Maximum feedrates (mm/s):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M203 X",max_feedrate[0]);
    SERIAL_ECHOPAIR(" Y",max_feedrate[1] ); 
    SERIAL_ECHOPAIR(" Z", max_feedrate[2] ); 
    SERIAL_ECHOPAIR(" E", max_feedrate[3]);
    SERIAL_ECHOLN("");

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Maximum Acceleration (mm/s2):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M201 X" ,max_acceleration_units_per_sq_second[0] ); 
    SERIAL_ECHOPAIR(" Y" , max_acceleration_units_per_sq_second[1] ); 
    SERIAL_ECHOPAIR(" Z" ,max_acceleration_units_per_sq_second[2] );
    SERIAL_ECHOPAIR(" E" ,max_acceleration_units_per_sq_second[3]);
    SERIAL_ECHOLN("");
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Acceleration: S=acceleration, T=retract acceleration");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M204 S",acceleration ); 
    SERIAL_ECHOPAIR(" T" ,retract_acceleration);
    SERIAL_ECHOLN("");

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Advanced variables: S=Min feedrate (mm/s), T=Min travel feedrate (mm/s), B=minimum segment time (ms), X=maximum XY jerk (mm/s),  Z=maximum Z jerk (mm/s),  E=maximum E jerk (mm/s)");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M205 S",minimumfeedrate ); 
    SERIAL_ECHOPAIR(" T" ,mintravelfeedrate ); 
    SERIAL_ECHOPAIR(" B" ,minsegmenttime ); 
    SERIAL_ECHOPAIR(" X" ,max_xy_jerk ); 
    SERIAL_ECHOPAIR(" Z" ,max_z_jerk);
    SERIAL_ECHOPAIR(" E" ,max_e_jerk);
    SERIAL_ECHOLN(""); 

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Home offset (mm):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("  M206 X",add_homeing[0] );
    SERIAL_ECHOPAIR(" Y" ,add_homeing[1] );
    SERIAL_ECHOPAIR(" Z" ,add_homeing[2] );
    SERIAL_ECHOLN("");
#ifdef PIDTEMP
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("PID settings:");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("   M301 P",Kp); 
    SERIAL_ECHOPAIR(" I" ,unscalePID_i(Ki)); 
    SERIAL_ECHOPAIR(" D" ,unscalePID_d(Kd));
    SERIAL_ECHOLN(""); 
#endif
*/
//    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR(" e_cavity_cool_pid_p:",e_cavity_cool_pid[P]);
    SERIAL_ECHOPAIR(" e_cavity_cool_pid_i:",e_cavity_cool_pid[I]);
    SERIAL_ECHOPAIR(" e_cavity_cool_pid_d:",e_cavity_cool_pid[D]);
    SERIAL_ECHOLN("");
    SERIAL_ECHOPAIR(" e_cavity_heat_pid_p:",e_cavity_heat_pid[P]);
    SERIAL_ECHOPAIR(" e_cavity_heat_pid_i:",e_cavity_heat_pid[I]);
    SERIAL_ECHOPAIR(" e_cavity_heat_pid_d:",e_cavity_heat_pid[D]);
    SERIAL_ECHOLN("");
    SERIAL_ECHOPAIR(" e_cavity_pipe_pid_p:",e_cavity_pipe_pid[P]);
    SERIAL_ECHOPAIR(" e_cavity_pipe_pid_i:",e_cavity_pipe_pid[I]);
    SERIAL_ECHOPAIR(" e_cavity_pipe_pid_d:",e_cavity_pipe_pid[D]);        
    SERIAL_ECHOLN("");
/*
    SERIAL_ECHOPAIR(" e0_cavity_cool_pid_p:",e0_cavity_cool_pid[P]);
    SERIAL_ECHOPAIR(" e0_cavity_cool_pid_i:",e0_cavity_cool_pid[I]);
    SERIAL_ECHOPAIR(" e0_cavity_cool_pid_d:",e0_cavity_cool_pid[D]);
    SERIAL_ECHOLN("");
    SERIAL_ECHOPAIR(" e0_cavity_heat_pid_p:",e0_cavity_heat_pid[P]);
    SERIAL_ECHOPAIR(" e0_cavity_heat_pid_i:",e0_cavity_heat_pid[I]);
    SERIAL_ECHOPAIR(" e0_cavity_heat_pid_d:",e0_cavity_heat_pid[D]);
    SERIAL_ECHOLN("");
    SERIAL_ECHOPAIR(" e0_cavity_pipe_pid_p:",e0_cavity_pipe_pid[P]);
    SERIAL_ECHOPAIR(" e0_cavity_pipe_pid_i:",e0_cavity_pipe_pid[I]);
    SERIAL_ECHOPAIR(" e0_cavity_pipe_pid_d:",e0_cavity_pipe_pid[D]);        
    SERIAL_ECHOLN("");
    SERIAL_ECHOPAIR(" e1_cavity_cool_pid_p:",e1_cavity_cool_pid[P]);
    SERIAL_ECHOPAIR(" e1_cavity_cool_pid_i:",e1_cavity_cool_pid[I]);
    SERIAL_ECHOPAIR(" e1_cavity_cool_pid_d:",e1_cavity_cool_pid[D]);
    SERIAL_ECHOLN("");
    SERIAL_ECHOPAIR(" e1_cavity_heat_pid_p:",e1_cavity_heat_pid[P]);
    SERIAL_ECHOPAIR(" e1_cavity_heat_pid_i:",e1_cavity_heat_pid[I]);
    SERIAL_ECHOPAIR(" e1_cavity_heat_pid_d:",e1_cavity_heat_pid[D]);
    SERIAL_ECHOLN("");
    SERIAL_ECHOPAIR(" e1_cavity_pipe_pid_p:",e1_cavity_pipe_pid[P]);
    SERIAL_ECHOPAIR(" e1_cavity_pipe_pid_i:",e1_cavity_pipe_pid[I]);
    SERIAL_ECHOPAIR(" e1_cavity_pipe_pid_d:",e1_cavity_pipe_pid[D]);        
    SERIAL_ECHOLN("");
*/      
    SERIAL_ECHOPAIR(" p_cool_pid_p:",p_cool_pid[P]);
    SERIAL_ECHOPAIR(" p_cool_pid_i:",p_cool_pid[I]);
    SERIAL_ECHOPAIR(" p_cool_pid_d:",p_cool_pid[D]);
    SERIAL_ECHOLN("");
    SERIAL_ECHOPAIR(" p_heat_pid_p:",p_heat_pid[P]);
    SERIAL_ECHOPAIR(" p_heat_pid_i:",p_heat_pid[I]);
    SERIAL_ECHOPAIR(" p_heat_pid_d:",p_heat_pid[D]);
    SERIAL_ECHOLN("");
    SERIAL_ECHOPAIR(" p_pipe_pid_p:",p_pipe_pid[P]);
    SERIAL_ECHOPAIR(" p_pipe_pid_i:",p_pipe_pid[I]);
    SERIAL_ECHOPAIR(" p_pipe_pid_d:",p_pipe_pid[D]);        
    SERIAL_ECHOLN("");      
} 
#endif


#ifdef EEPROM_SETTINGS
void Config_RetrieveSettings()
{
    int i=EEPROM_OFFSET;
    char stored_ver[4];
    char ver[4]=EEPROM_VERSION;
    EEPROM_READ_VAR(i,stored_ver); //read stored version
    //SERIAL_ECHOLN("Version: [" << ver << "] Stored version: [" << stored_ver << "]");
    if (strncmp(ver,stored_ver,3) == 0)
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
        EEPROM_READ_VAR(i,e_cavity_cool_pid[P]);
        EEPROM_READ_VAR(i,e_cavity_cool_pid[I]);
        EEPROM_READ_VAR(i,e_cavity_cool_pid[D]);
        EEPROM_READ_VAR(i,e_cavity_heat_pid[P]);
        EEPROM_READ_VAR(i,e_cavity_heat_pid[I]);
        EEPROM_READ_VAR(i,e_cavity_heat_pid[D]);
        EEPROM_READ_VAR(i,e_cavity_pipe_pid[P]);
        EEPROM_READ_VAR(i,e_cavity_pipe_pid[I]);
        EEPROM_READ_VAR(i,e_cavity_pipe_pid[D]);
/*        
        EEPROM_READ_VAR(i,e0_cavity_cool_pid[P]);
        EEPROM_READ_VAR(i,e0_cavity_cool_pid[I]);
        EEPROM_READ_VAR(i,e0_cavity_cool_pid[D]);
        EEPROM_READ_VAR(i,e0_cavity_heat_pid[P]);
        EEPROM_READ_VAR(i,e0_cavity_heat_pid[I]);
        EEPROM_READ_VAR(i,e0_cavity_heat_pid[D]);
        EEPROM_READ_VAR(i,e0_cavity_pipe_pid[P]);
        EEPROM_READ_VAR(i,e0_cavity_pipe_pid[I]);
        EEPROM_READ_VAR(i,e0_cavity_pipe_pid[D]);

        EEPROM_READ_VAR(i,e1_cavity_cool_pid[P]);
        EEPROM_READ_VAR(i,e1_cavity_cool_pid[I]);
        EEPROM_READ_VAR(i,e1_cavity_cool_pid[D]);
        EEPROM_READ_VAR(i,e1_cavity_heat_pid[P]);
        EEPROM_READ_VAR(i,e1_cavity_heat_pid[I]);
        EEPROM_READ_VAR(i,e1_cavity_heat_pid[D]);
        EEPROM_READ_VAR(i,e1_cavity_pipe_pid[P]);
        EEPROM_READ_VAR(i,e1_cavity_pipe_pid[I]);
        EEPROM_READ_VAR(i,e1_cavity_pipe_pid[D]); 
*/
        EEPROM_READ_VAR(i,p_cool_pid[P]);
        EEPROM_READ_VAR(i,p_cool_pid[I]);
        EEPROM_READ_VAR(i,p_cool_pid[D]);
        EEPROM_READ_VAR(i,p_heat_pid[P]);
        EEPROM_READ_VAR(i,p_heat_pid[I]);
        EEPROM_READ_VAR(i,p_heat_pid[D]);
        EEPROM_READ_VAR(i,p_pipe_pid[P]);
        EEPROM_READ_VAR(i,p_pipe_pid[I]);
        EEPROM_READ_VAR(i,p_pipe_pid[D]);                                  
        // wp add end

		// Call updatePID (similar to when we have processed M301)
		    updatePID();
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM("Stored settings retrieved");
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
    float _e_cavity_cool_pid[] = E_CAVITY_COOL_PID;
    e_cavity_cool_pid[P] = _e_cavity_cool_pid[P];
    e_cavity_cool_pid[I] = _e_cavity_cool_pid[I];
    e_cavity_cool_pid[D] = _e_cavity_cool_pid[D];
    float _e_cavity_heat_pid[] = E_CAVITY_HEAT_PID;
    e_cavity_heat_pid[P] = _e_cavity_heat_pid[P];
    e_cavity_heat_pid[I] = _e_cavity_heat_pid[I];
    e_cavity_heat_pid[D] = _e_cavity_heat_pid[D]; 
    float _e_cavity_pipe_pid[] = E_CAVITY_PIPE_PID;
    e_cavity_pipe_pid[P] = _e_cavity_pipe_pid[P];
    e_cavity_pipe_pid[I] = _e_cavity_pipe_pid[I];
    e_cavity_pipe_pid[D] = _e_cavity_pipe_pid[D]; 
/*    
    float _e0_cavity_cool_pid[] = E0_CAVITY_COOL_PID;
    e0_cavity_cool_pid[P] = _e0_cavity_cool_pid[P];
    e0_cavity_cool_pid[I] = _e0_cavity_cool_pid[I];
    e0_cavity_cool_pid[D] = _e0_cavity_cool_pid[D];
    float _e0_cavity_heat_pid[] = E0_CAVITY_HEAT_PID;
    e0_cavity_heat_pid[P] = _e0_cavity_heat_pid[P];
    e0_cavity_heat_pid[I] = _e0_cavity_heat_pid[I];
    e0_cavity_heat_pid[D] = _e0_cavity_heat_pid[D]; 
    float _e0_cavity_pipe_pid[] = E0_CAVITY_PIPE_PID;
    e0_cavity_pipe_pid[P] = _e0_cavity_pipe_pid[P];
    e0_cavity_pipe_pid[I] = _e0_cavity_pipe_pid[I];
    e0_cavity_pipe_pid[D] = _e0_cavity_pipe_pid[D]; 

    float _e1_cavity_cool_pid[] = E1_CAVITY_COOL_PID;
    e1_cavity_cool_pid[P] = _e1_cavity_cool_pid[P];
    e1_cavity_cool_pid[I] = _e1_cavity_cool_pid[I];
    e1_cavity_cool_pid[D] = _e1_cavity_cool_pid[D];
    float _e1_cavity_heat_pid[] = E1_CAVITY_HEAT_PID;
    e1_cavity_heat_pid[P] = _e1_cavity_heat_pid[P];
    e1_cavity_heat_pid[I] = _e1_cavity_heat_pid[I];
    e1_cavity_heat_pid[D] = _e1_cavity_heat_pid[D]; 
    float _e1_cavity_pipe_pid[] = E1_CAVITY_PIPE_PID;
    e1_cavity_pipe_pid[P] = _e1_cavity_pipe_pid[P];
    e1_cavity_pipe_pid[I] = _e1_cavity_pipe_pid[I];
    e1_cavity_pipe_pid[D] = _e1_cavity_pipe_pid[D];
*/
    float _p_cool_pid[] = P_COOL_PID;
    p_cool_pid[P] = _p_cool_pid[P];
    p_cool_pid[I] = _p_cool_pid[I];
    p_cool_pid[D] = _p_cool_pid[D];
    float _p_heat_pid[] = P_HEAT_PID;
    p_heat_pid[P] = _p_heat_pid[P];
    p_heat_pid[I] = _p_heat_pid[I];
    p_heat_pid[D] = _p_heat_pid[D]; 
    float _p_pipe_pid[] = P_PIPE_PID;
    p_pipe_pid[P] = _p_pipe_pid[P];
    p_pipe_pid[I] = _p_pipe_pid[I];
    p_pipe_pid[D] = _p_pipe_pid[D];    
    // wp add end
SERIAL_ECHO_START;
SERIAL_ECHOLNPGM("Hardcoded Default Settings Loaded");

}
