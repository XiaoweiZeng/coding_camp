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

int catch_action(int * param)
{
    char cmd_1[ACTION_LENGTH];
    char cmd_2[ACTION_SINGLE_LENGTH];

    if (param == NULL)
    {
    printf(" param is NULL\n");
    return -1;
    }

    /* reset */
    uart_send(reset_abs_6, strlen(reset_abs_6));
    usleep(DEFAULT_TIMEOUT * 1000);

    /* convert param to command */
    param_to_cmd(cmd_1, cmd_2, param);
    printf("catch command:\n\tcmd_1=%s\n\tcmd_2=%s\n", cmd_1, cmd_2);

    /* execute cmd claw */
    uart_send(cmd_1, strlen(cmd_1));
    usleep(DEFAULT_TIMEOUT * 1000);
    uart_send(cmd_2, strlen(cmd_2));
    usleep(DEFAULT_TIMEOUT * 1000);

    /* reset without */
    uart_send(reset_abs_5, strlen(reset_abs_5));
    usleep(DEFAULT_TIMEOUT * 1000);
}

int release_action(int * param)
{
    char cmd_1[ACTION_LENGTH];
    char cmd_2[ACTION_SINGLE_LENGTH];

    if (param == NULL)
    {
    printf(" param is NULL\n");
    return -1;
    }

    /* convert param to command */
    param_to_cmd(cmd_1, cmd_2, param);
    printf("release command:\n\tcmd_1=%s\n\tcmd_2=%s\n", cmd_1, cmd_2);

    /* execute cmd claw */
    uart_send(cmd_1, strlen(cmd_1));
    usleep(DEFAULT_TIMEOUT * 1000);
    uart_send(cmd_2, strlen(cmd_2));
    usleep(DEFAULT_TIMEOUT * 1000);

    /* reset without */
    uart_send(reset_abs_6, strlen(reset_abs_6));
    usleep(DEFAULT_TIMEOUT * 1000);
}

int main(int argc, char *argv[])
{
    int param_catch[7] = {0, -1, 1300, 1700, 1800, -1, 2000};
    char cmd_1[ACTION_LENGTH];
    char cmd_2[ACTION_SINGLE_LENGTH];

    if (uart_init() < 0)
    {
    printf("uart init failed.\n");
    return -1;
    }
    servo_self_check();

    catch_action(param_catch);


    return 0;
}


