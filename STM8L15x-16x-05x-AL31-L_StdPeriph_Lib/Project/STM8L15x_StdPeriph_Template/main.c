/**
 * @file main.c
 * @brief Main implementation for the nixie watch firmware
 */

/* Includes */
#include "stm8l15x.h"
#include <string.h>
#include "hardwaredefs.h"
#include "nixie.h"
#include "uart.h"
#include "ext_rtc.h"
#include "state_machine.h"

void main(void)
{
  /* High speed internal clock prescaler: 1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
  /* Initialize mounted on board */
  GPIO_Init(LED_GPIO_PORT, LED_GPIO_PINS, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_ExternalPullUpConfig(LED_GPIO_PORT, LED_GPIO_PINS, ENABLE);

  sm_configure_interrupts(&state_machine);

  enableInterrupts();

  /* Configure UART module */
  #ifdef STM8_BASEBAND
  init_uart();
  ext_rtc_init();
  #endif /* STM8_BASEBAND */

  nixie_init_pins(&tube_A, &shared_psu);


  /* Main loop */
  while (1)
  {

    /* Check new state machine request */
    sm_execute_requests(&state_machine, &state_machine_request);

    /* Enter wait for interrupt mode (turns off CPU to save power) (Page 73 of TRM doc # RM0031) */
    wfi();
  }
}

