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

#include     "uart.h"

#define DEFAULT_TIMEOUT         1500
#define ACTION_LENGTH           70
#define ACTION_SINGLE_LENGTH    20

#undef UART_DEBUG

static int uart_id;

typedef struct arm_args {
    int  abs_x;
    int  abs_y;
    int  timeout;
    char action[ACTION_LENGTH];
} ARM_ARGS;

static void usage(void)
{
    fprintf(stderr, "Usage: encode [options]\n\n"
                    "Options:\n"
                    "-x | --x_abs       X-axis\n"
                    "-y | --y_abs       Y-axis\n"
                    "-t | --time        complete time.(ms)\n"
                    "-a | --action      action data"
                    "-h | --help        help message\n");
}

static void parse_args(int argc, char *argv[], ARM_ARGS * arm_args)
{
    int index;
    int cmd;

    const char params[] = "x:y:t:s:h";
    const struct option long_options[] =
    {
        {"x_abs",       required_argument, NULL, 'x'},
        {"y_abs",       required_argument, NULL, 'y'},
        {"timeout",     required_argument, NULL, 't'},
        {"help",        no_argument,       NULL, 'h'},
        {0, 0, 0, 0}
    };

    for (;;)
    {
        cmd = getopt_long(argc, argv, params, long_options, &index);

        if (cmd == -1)
        {
            if ((arm_args->abs_x < 0) || (arm_args->abs_y < 0))
            {
                fprintf(stderr, "axis value should bigger than 0.\n");
                exit(EXIT_FAILURE);
            }
            if (arm_args->timeout < 500)
            {
                fprintf(stderr, "timeout should bigger than 500.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }

        switch (cmd)
        {
        case 'x':
            arm_args->abs_x = atoi(optarg);
            break;
        case 'y':
            arm_args->abs_y = atoi(optarg);
            break;
        case 't':
            arm_args->timeout = atoi(optarg);
            break;
        case 's':
            memcpy(arm_args->action, optarg, sizeof(optarg));
            break;
        case 'h':
            usage();
            exit(EXIT_SUCCESS);
            break;
        default:
            usage();
            exit(EXIT_FAILURE);
            break;
        }
    }
}

int uart_init(void)
{
   struct termios options;

   uart_id = open(UART_DEVICE, O_RDWR | O_NOCTTY);
   if (uart_id < 0)
    {
        fprintf(stderr, "can not open device %s\n", UART_DEVICE);
        return -1;
    }

    /* set uart parameters */

    tcgetattr(uart_id, &options);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;


    options.c_cflag |= (CLOCAL);
    options.c_cflag &= ~CRTSCTS;
    options.c_iflag |= IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    cfsetospeed(&options, B115200);
    tcflush(uart_id, TCIFLUSH);
    tcsetattr(uart_id, TCSANOW, &options);

    fprintf(stdout, "uart init complete.\n");

    return 0;
}

int uart_send(char *data, int datalen)
{
    int len = 0;
    int i;

    if (data == NULL)
        fprintf(stderr, "data pointer is NULL\n");

    len = write(uart_id, data, datalen);
    if (len != datalen)
    {
        fprintf(stderr, "should send %d, actually %d\n", datalen, len);
        tcflush(uart_id, TCOFLUSH);
        return -1;
    }
    else
    {
        fprintf(stdout, "[send]:");
        for (i = 0; i < datalen; i++)
        {
        fprintf(stdout, "%c", *(data + i));
        }
        fprintf(stdout, "\n");
    }

    return 0;
}

void uart_close(void)
{
    close(uart_id);
}

#ifdef UART_DEBUG

int main(int argc, char *argv[])
{
    ARM_ARGS arm_args;
    char action[ACTION_LENGTH] = "#1P1700#2P1500#3P1700#4P1800#5P1500#6P2000T2000!";
    int param[7] = {0, 1400, 1500, 1600, -1, 1700, 2000};
    char cmd_1[ACTION_LENGTH];
    char cmd_2[ACTION_SINGLE_LENGTH];

    arm_args.abs_x   = INVALID_AXIS_VALUE;
    arm_args.abs_y   = INVALID_AXIS_VALUE;
    arm_args.timeout = DEFAULT_TIMEOUT;
    arm_args.action[0] = 0;


    param_to_cmd(cmd_1, cmd_2, param);
    printf("cmd_1=%s, cmd_2=%s\n", cmd_1, cmd_2);

    /* Parse the arguments */

    parse_args(argc, argv, &arm_args);

    if (uart_init() < 0)
        return -1;

    if (arm_args.action != NULL)
        uart_send(uart_id, arm_args.action, sizeof(arm_args.action)-1);

    uart_send(uart_id,
              "#1P1700#2P1500#3P1700#4P1800#5P1500#6P2000T2000!",
              strlen("#1P1700#2P1500#3P1700#4P1800#5P1500#6P2000T2000!"));




    //sleep(5);
    close(uart_id);


    return 0;

}
#endif






