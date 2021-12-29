/**
 * @file main.c
 * @brief Main implementation for RTC based timer (print over USB)
 */

/* Includes */
#include "stm8l15x.h"
#include <string.h>
#include "hardwaredefs.h"
#include "nixie.h"
#include "uart.h"
#include "ext_rtc.h"
#include "state_machine.h"

/* Private function prototypes */
void Delay (uint16_t nCount);


void main(void)
{
  /* High speed internal clock prescaler: 1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
  /* Initialize LEDs mounted on STM8L152X-EVAL board */
  GPIO_Init(LED_GPIO_PORT, LED_GPIO_PINS, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_ExternalPullUpConfig(LED_GPIO_PORT, LED_GPIO_PINS, ENABLE);

  sm_configure_interrupts(&state_machine);

  enableInterrupts();

  /* Configure UART module */
  init_uart();
  ext_rtc_init();

  nixie_init_pins(&tube_A, &shared_psu);
  nixie_enable_psu(&shared_psu);


  /* Main loop */
  while (1)
  {

    /* Check new state machine request */
    sm_execute_requests(&state_machine, &state_machine_request);

    /* Enter wait for interrupt mode (turns off CPU to save power) (Page 73 of TRM doc # RM0031) */
    wfi();
  }
}

/**
 * @brief  Inserts a delay time.
 * @param  nCount: specifies the delay time length.
 * @retval None
 */
void Delay(__IO uint16_t nCount)
{
  /* Decrement nCount value */
	while (nCount != 0)
  {
    nCount--;
  }
}

