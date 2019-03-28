#ifndef _ARMCTRL_H_
#define _ARMCTRL_H_

#define DEFAULT_TIMEOUT         1500
#define ACTION_LENGTH           70
#define ACTION_SINGLE_LENGTH    20


extern const char reset_abs[];
extern const char * servo_self_check_data[18];

int param_to_cmd(char *cmd, char *cmd_catch,int * param);

#endif
