/**
 * @file ext_rtc.h
 * @brief Function prototypes, defines and types for external RTC (baseband board only)
 */

#ifndef EXT_RTC_H_
#define EXT_RTC_H_

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

/* I2C config data */
#define I2C_SPEED 100000
#define I2C_OWN_ADDRESS 0xA0

/* External RTC addressing data */
#define RTC_I2C_ADDRESS 0b11010000
#define RTC_SECS_ADDR 0x00
#define RTC_MINS_ADDR 0x01
#define RTC_CLEAR_NV 0x00

/* Useful macro for reducing runtime code size */
#define ARR_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* RTC bitmasks */
#define TEN_SEC_BITMASK 0b01110000
#define SECONDS_BITMASK 0b00001111

/* RTC data payload read size */
#define RTC_PAY_READ_SIZE 2

/* Output data string buffer size */
#define RTC_HOST_PRINT_SIZE 3

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

/**
 * @brief RTC digit print type
 */
typedef enum
{
  RTC_PRINT_SECONDS,

  RTC_PRINT_MINUTES,

  RTC_PRINT_HOURS

} print_type_t;

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/

void ext_rtc_init(void);
void ext_rtc_write(uint8_t addr, uint8_t data);
void ext_rtc_read(uint8_t* bytes, uint8_t len);
uint8_t ext_rtc_decode(uint8_t val);
void ext_rtc_print_val(uint8_t val, print_type_t type);

#endif /* EXT_RTC_H_ */
