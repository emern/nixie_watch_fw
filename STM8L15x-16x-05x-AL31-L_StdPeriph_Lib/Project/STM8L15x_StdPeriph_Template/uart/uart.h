/**
 * @file uart.h
 * @brief Function prototypes for UART-to-host communication library
 */

#ifndef UART_H_
#define UART_H_

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/
#define UART_BAUDRATE 115200

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/
void init_uart(void);
char putchar(char c);
char getchar(void);
void tiny_print(char* str, int len);
void tiny_scan(char* out, int len);

#endif /* UART_H_ */
