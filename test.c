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
#include     "abs_caculator.h"

static void servo_self_check(void)
{
    int i;
    for (i = 0; i < 18; i++)
    {
        uart_send((char *)servo_self_check_data[i], strlen(servo_self_check_data[i]));
        usleep(WAIT_TIMEOUT * 1000);
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
    usleep(WAIT_TIMEOUT * 1000);

    /* convert param to command */
    param_to_cmd(cmd_1, cmd_2, param);
    printf("catch command:\n\tcmd_1=%s\n\tcmd_2=%s\n", cmd_1, cmd_2);

    /* execute cmd claw */
    uart_send(cmd_1, strlen(cmd_1));
    usleep(WAIT_TIMEOUT * 1000);
    uart_send(cmd_2, strlen(cmd_2));
    usleep(WAIT_TIMEOUT * 1000);

    /* reset without */
    uart_send(reset_abs_5, strlen(reset_abs_5));
    usleep(WAIT_TIMEOUT * 1000);
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
    usleep((2000) * 1000);
    uart_send(cmd_2, strlen(cmd_2));
    usleep(2000 * 1000);

    /* reset without */
    uart_send(reset_abs_6, strlen(reset_abs_6));
    usleep(DEFAULT_TIMEOUT * 1000);
}

int main(int argc, char *argv[])
{
    int param_catch_1[7] = { 0, 1278, 1153, 1823, 2141, 1380, 1580};
    int param_catch_2[7] = { 0, 1401, 1160, 1819, 2204, 1457, 1580};
    int param_catch_3[7] = { 0, 1577, 1195, 1887, 2166, 1535, 1580};
    int param_catch_4[7] = { 0, 1734, 1092, 1704, 2234, 1813, 1580};
    int param_release[7] = { 0, 1000, 1195, 1887, 2166, 1535, 1300};
    char cmd_1[ACTION_LENGTH];
    char cmd_2[ACTION_SINGLE_LENGTH];
    int * abs;

    if (uart_init() < 0)
    {
    printf("uart init failed.\n");
    return -1;
    }
    //servo_self_check();


   uart_send("#Veri+201903291448\r\n", strlen("#Veri+201903291448\r\n"));
   usleep(WAIT_TIMEOUT * 1000);
/*
   abs = getPosture(0, 0.2);
   if (abs == NULL)
       {
	printf("abs is NULL\n");
	return -1;
	}
*/
   //catch_action(abs);


   catch_action(param_catch_1);
   release_action(param_release);

   catch_action(param_catch_2);
   release_action(param_release);

   catch_action(param_catch_3);
   release_action(param_release);

   catch_action(param_catch_4);
   release_action(param_release);
 /*

   uart_send("#2P742T2000\r\n", strlen("#2P742T2000\r\n"));
    usleep(WAIT_TIMEOUT * 1000);


    uart_send("#3P1161T2000\r\n", strlen("#2P1161T2000\r\n"));
    usleep(WAIT_TIMEOUT * 1000);

    uart_send("#4P2500T2000\r\n", strlen("#2P2500T2000\r\n"));
    usleep(WAIT_TIMEOUT * 1000);

    */




    return 0;
}


