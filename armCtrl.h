#ifndef _ARMCTRL_H_
#define _ARMCTRL_H_

#define DEFAULT_TIMEOUT         2000
#define WAIT_TIMEOUT            3000

#define ACTION_LENGTH           70
#define ACTION_SINGLE_LENGTH    20


extern char reset_abs_5[];
extern char reset_abs_6[];

extern const char * servo_self_check_data[18];

int param_to_cmd(char *cmd, char *cmd_catch,int * param);

#endif
