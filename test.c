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
#include     "uart.h"

static void servo_self_check(void)
{
    int i;
    for (i = 0; i < 18; i++)
    {
        uart_send((char *)servo_self_check_data[i], strlen(servo_self_check_data[i]));
        usleep(800000);
    }
}

int main(int argc, char *argv[])
{
    int param_test1[7] = {0, -1, 1300, 1700, 1800, -1, 2000};
    char cmd_1[ACTION_LENGTH];
    char cmd_2[ACTION_SINGLE_LENGTH];

    uart_init();
    servo_self_check();

    param_to_cmd(cmd_1, cmd_2, param_test1);
    printf("cmd_1=%s\ncmd_2=%s\n", cmd_1, cmd_2);

    uart_send(cmd_1, strlen(cmd_1));
    usleep(DEFAULT_TIMEOUT * 1000);
    uart_send(cmd_2, strlen(cmd_2));
    usleep(DEFAULT_TIMEOUT * 1000);

    return 0;
}


