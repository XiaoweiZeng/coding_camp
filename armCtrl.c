#include     <stdio.h>
#include     <stdlib.h>
#include     <unistd.h>
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>
#include     <termios.h>
#include     <errno.h>
#include     <getopt.h>
#include     <string.h>

#include     "armCtrl.h"

char reset_abs_5[] = "#1P1500#2P1500#3P1500#4P1500#5P1500T1500!";
char reset_abs_6[] = "#1P1500#2P1500#3P1500#4P1500#5P1500#6P1500T1500!";

const char * servo_self_check_data[18] =
{
    "#1P1000T700!",
    "#1P2000T700!",
    "#1P1500T700!",

    "#2P1700T700!",
    "#2P1300T700!",
    "#2P1500T700!",

    "#3P1800T700!",
    "#3P1200T700!",
    "#3P1500T700!",

    "#4P1900T700!",
    "#4P1100T700!",
    "#4P1500T700!",

    "#5P2000T700!",
    "#5P1000T700!",
    "#5P1500T700!",

    "#6P2200T700!",
    "#6P800T700!",
    "#6P1500T700!",
};

int param_to_cmd(char *cmd, char *cmd_catch,int * param)
{
    int i;
    char cmd_single[ACTION_SINGLE_LENGTH];

    /* validate cmd and param */

    if ((cmd == NULL) || (param == NULL))
    {
        fprintf(stderr, "cmd or param pointer is NULL\n");
        return -1;
    }

    memset(cmd,       0, ACTION_LENGTH);
    memset(cmd_catch, 0, ACTION_SINGLE_LENGTH);

    /* for first 5 servos */

    for (i = 1; i < 6; i++)
    {
        if (param[i] != -1)
        {
            snprintf(cmd_single, sizeof(cmd_single), "#%dP%d", i, param[i]);
            strcat(cmd, cmd_single);
        }
    }
    if (cmd[0] != 0)
    {
    snprintf(cmd_single, sizeof(cmd_single), "T%d!", DEFAULT_TIMEOUT);
    strcat(cmd, cmd_single);
    }

    if (param[6] != -1)
    {
        snprintf(cmd_catch, sizeof(cmd_single), "#6P%dT%d!", param[6], DEFAULT_TIMEOUT/2);
    }

    return 0;
}
