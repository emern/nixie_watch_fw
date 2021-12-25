/**
 * @file ext_rtc.c
 * @brief Implementations for external RTC slave device communication (via I2C) and host communication via UART
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/
#include "stm8l15x_i2c.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_clk.h"

#include "hardwaredefs.h"
#include "ext_rtc.h"
#include "uart.h"

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void ext_rtc_init(void)
{
  /* Enable I2C module */
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);
  I2C_Init(I2C1, I2C_SPEED, I2C_OWN_ADDRESS, I2C_Mode_I2C, I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
  I2C_Cmd(I2C1, ENABLE);

  /* Configure I2C GPIO to HiZ (board has external 10k pullups) */
  GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_Mode_Out_OD_HiZ_Fast);
  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_OD_HiZ_Fast);

  /* Must write 0 to RTC to disable the Clock Halt bit and clear data stored in NV (seconds) */
  ext_rtc_write(RTC_SECS_ADDR, RTC_CLEAR_NV);
  /* Clear NV (minutes) */
  ext_rtc_write(RTC_SECS_ADDR, RTC_CLEAR_NV);
}

/**
 * @brief Write byte to RTC register addr
 * @param addr: RTC register address to write data byte to
 * @param data: Data to write
*/
void ext_rtc_write(uint8_t addr, uint8_t data)
{
  /* Generate I2C start condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send RTC device address */
  I2C_Send7bitAddress(I2C1, RTC_I2C_ADDRESS, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Send target register address */
  I2C_SendData(I2C1, addr);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send data byte */
  I2C_SendData(I2C1, data);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Generate stop condition */
  I2C_GenerateSTOP(I2C1, ENABLE);

  return;
}

/**
 * @brief Transmit starting address and then act as master reciever
 *
 * Example on multibyte data reads avaliable in section 28.4.2 (fig 147) of Doc # RM0031 STM8L TRM
*/
void ext_rtc_read(uint8_t* bytes, uint8_t len)
{
  uint8_t i;

  /* Generate I2C start event, (EV5) */
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send slave device address, (EV6) */
  I2C_Send7bitAddress(I2C1, RTC_I2C_ADDRESS, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Send register read address, in this case we start read from RTC base register (address 0) (EV_8) */
  I2C_SendData(I2C1, 0);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Generate repeated start condition (EV5) */
  I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send slave device address, (EV6) */
  I2C_Send7bitAddress(I2C1, RTC_I2C_ADDRESS, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  /* Not actually in the datasheet but we must explicitly enable I2C ACK in this stage */
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  /* If only a single byte is being read, prepare the stop condition */
  if (len == 1)
  {
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
  }

  /* Read i bytes from RTC, on second to last data read we must set a NACK and set a STOP condition */
  for (i=0; i<len; i++)
  {
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    bytes[i] = I2C_ReceiveData(I2C1);

    /* Case for if i = 1 already handled by pre-loop statement */
    if (i == (len - 2))
    {
      /* Disable ACK to generate final NACK and STOP conditions */
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      I2C_GenerateSTOP(I2C1, ENABLE);
    }
  }
}

/**
 * @brief Function to decode BCD RTC value to decimal
 * @param val: Raw encoded value
 * @retval Decoded decimal time value
*/
uint8_t ext_rtc_decode(uint8_t val)
{
  uint8_t ten_ticks = (val >> 4) * 10;
  uint8_t ticks = val & SECONDS_BITMASK;
  return ten_ticks + ticks;
}

/**
 * @brief Function to print BCD encoded RTC value to Host PC
 * @param val: Encoded RTC value
 * @param type: Data type indicator
*/
void ext_rtc_print_val(uint8_t val, print_type_t type)
{
  char out[3];
  const char secs_header[] = "Seconds: ";
  const char mins_header[] = "Minutes: ";
  const char newline[] = "\r\n";
  const char line_break[] = "\r\n\r\n";

  /**
   * Convert BCD to ASCII by bit manipulations.
   *
   * Step 1) Extract "10's" digit by recovering only bits 4:6 of the RTC value to out[0]
   * Step 2) Extract "1's" digit by bitmasking the lower 4 bits of RTC value to out[1]
   * Step 3) Convert decimal digits to equivalent ASCII interpretation by adding constant 48
   *
   * Note: tiny_print expects NULL terminated string, must append NULL character for databuffer
   */
  out[0] = (char)((val >> 4) + 48);
  out[1] = (char)((val & SECONDS_BITMASK) + 48);
  out[2] = '\0';

  /* Print based on type */
  switch (type)
  {
  case RTC_PRINT_SECONDS:
    tiny_print(secs_header, ARR_SIZE(secs_header));
    tiny_print(out, RTC_HOST_PRINT_SIZE);
    tiny_print(newline, ARR_SIZE(newline));
    break;

  case RTC_PRINT_MINUTES:
    tiny_print(mins_header, ARR_SIZE(mins_header));
    tiny_print(out, RTC_HOST_PRINT_SIZE);
    tiny_print(line_break, ARR_SIZE(line_break));
    break;

  default:
    break;
  }
}
