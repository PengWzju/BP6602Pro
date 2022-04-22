#ifndef _TC_SELF_CHECK
#define _TC_SELF_CHEK

bool check_extruder();
bool check_platform();
bool check_extruder_max_temp();
bool check_platform_max_temp();

void check_init();
void max_temp_check();

#endif
