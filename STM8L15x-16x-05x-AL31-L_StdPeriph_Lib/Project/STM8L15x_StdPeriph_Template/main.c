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

/* Private function prototypes */
void Delay (uint16_t nCount);


void main(void)
{
  /* Initialize system variables */
  uint8_t i2c_data[] = {0,0};
  uint8_t secs_dec = 0;
  uint8_t secs_dec_last = 0;

  /* High speed internal clock prescaler: 1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
  /* Initialize LEDs mounted on STM8L152X-EVAL board */
  GPIO_Init(LED_GPIO_PORT, LED_GPIO_PINS, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_ExternalPullUpConfig(LED_GPIO_PORT, LED_GPIO_PINS, ENABLE);

  /* Configure UART module */
  init_uart();
  ext_rtc_init();

  nixie_init_pins(&tube_A, &shared_psu);
  nixie_enable_psu(&shared_psu);


  /* Main loop */
  while (1)
  {

    /* Read rtc data */
    ext_rtc_read(i2c_data, RTC_PAY_READ_SIZE);
    /* Decode number of seconds recieved (always first byte) to determine if print is needed */
    secs_dec = ext_rtc_decode(i2c_data[0]);

    /* Print time data if new second has rolled over */
    if (secs_dec != secs_dec_last)
    {
      secs_dec_last = secs_dec;
      ext_rtc_print_val(i2c_data[0], RTC_PRINT_SECONDS);
      ext_rtc_print_val(i2c_data[1], RTC_PRINT_MINUTES);
      nixie_digit_control(&tube_A, secs_dec % 10, DIGIT_ON, &shared_psu);
    }

    /* Toggle on board GPIO and delay */
    GPIO_ToggleBits(LED_GPIO_PORT, LED_GPIO_PINS);
    Delay(0xFF00);
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

