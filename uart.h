#ifndef _UART_H_
#define _UART_H_

#define UART_DEVICE     "/dev/ttyUSB0"
#define INVALID_AXIS_VALUE  -1

int uart_init(void);
int uart_send(char *data, int datalen);
void uart_close(void);

#endif
