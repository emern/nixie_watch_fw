/**
 * @file uart.c
 * @brief Function prototypes for UART-to-host communication library
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/
#include "stm8l15x_usart.h"
#include "stm8l15x_clk.h"

#include "hardwaredefs.h"
#include "uart.h"
#include <string.h>

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

/**
 * @brief Initialization routine needed for device-to-host communication via USB-UART
 */
void init_uart(void)
{
  /* Must remap the UART1 default pin config */
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, ENABLE);

  /* USART1 CLK enable */
	CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);

  //GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_3, ENABLE);

  /* Enable UART TX */
  USART_Init(USART1, UART_BAUDRATE, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx));
  USART_ClockInit(USART1, USART_Clock_Disable, USART_CPOL_Low, USART_CPHA_1Edge, USART_LastBit_Disable);
  USART_Cmd(USART1, ENABLE);
}

/**
 * @brief Send byte to host via UART
 * @param c: Byte to send
 * @retval Byte sent
*/
char putchar(char c)
{
  /* Write a character to the UART1 */
  USART_SendData8(USART1, c);
/* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  return (c);
}

/**
 * @brief Recieve byte from host via UART
 * @retval Byte recieved
 * @note This operation is blocking, if host does not send any data, device will poll indefinitely
*/
char getchar(void)
{
  char c = 0;
  /* Loop until the Read data register flag is SET */
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
  c = USART_ReceiveData8(USART1);
  return (c);
}

/**
 * @brief Print pre-formatted string via UART
 * @param str: Pre-formatted string to print
 * @param len: Length of string INCLUDING NULL TERMINATOR
 * @retval None
 *
 * @note In order to reduce compiled code size, the ARR_SIZE macro is reccomended
 *       to be used to calculate the len parameter at compile time. Since ARR_SIZE
 *       calculates complete buffer size, NULL terminator is expected in the value
 *       of len.
*/
void tiny_print(char* str, int len)
{
  int i;

  /* Double check string length */
  if (strlen(str) != len - 1)
  {
    return;
  }

  /* Send string via UART */
  for (i=0; i<len; i++)
  {
    putchar(str[i]);
  }
}

/**
 * @brief Scan for string of length len via UART
 * @param out: Data buffer containing recieved string
 * @param len: Length of expected string INCLUDING NULL TERMINATOR
 * @retval None
 *
 * @note In order to reduce compiled code size, the ARR_SIZE macro is reccomended
 *       to be used to calculate the len parameter at compile time. Since ARR_SIZE
 *       calculates complete buffer size, NULL terminator is expected in the value
 *       of len.
*/
void tiny_scan(char* out, int len)
{
  int i;

  if (strlen(out) != len - 1)
  {
    return;
  }

  for (i=0; i<len; i++)
  {
    out[i] = getchar();
  }
}
