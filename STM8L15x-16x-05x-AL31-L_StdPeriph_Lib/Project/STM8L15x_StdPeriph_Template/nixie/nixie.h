/**
 * @file nixie.h
 * @brief Contains structs and API prototypes for nixie tube drivers
 */

#ifndef NIXIE_H_
#define NIXIE_H_


/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/
#include "stm8l15x_gpio.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/
#define NUM_NIXIE_DIGITS 10

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

/**
 * @brief Nixie tube error type enumeration
 */
typedef enum
{
    NIXIE_ERROR = 0,

    NIXIE_PSU_DISABLED,

    NIXIE_SUCCESS

} nixie_error_t;

/**
 * @brief Nixie digit status enumeration
 */
typedef enum
{
    DIGIT_ON = 0,

    DIGIT_OFF

} nixie_digit_state_t;

/**
 * @brief Nixie tube digit struct, contains digit addressing information as well as current state
 */
typedef struct
{
    nixie_digit_state_t curr_state;

    GPIO_TypeDef* gpio_port;

    uint8_t gpio_pin;

} nixie_char_t;

/**
 * @brief Nixie tube power supply object
 */
typedef struct
{
    bool psu_enabled;

    GPIO_TypeDef* psu_port;

    uint8_t psu_pin;

} nixie_psu_t;


/**
 * @brief Nixie tube struct, contains addressing and status information for all digits along with power supply
 */
typedef struct
{
    nixie_char_t digits[NUM_NIXIE_DIGITS];

} nixie_tube_t;

/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/
extern nixie_tube_t tube_A;
extern nixie_psu_t shared_psu;

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/
/* Initialization */
void nixie_init_pins(nixie_tube_t* tube, nixie_psu_t* psu);

/* Power supply control */
void nixie_enable_psu(nixie_psu_t* psu);
void nixie_disable_psu(nixie_psu_t* psu);

/* Digit control */
nixie_error_t nixie_digit_control(nixie_tube_t* tube, uint8_t digit, nixie_digit_state_t state, nixie_psu_t* psu);

#endif /* NIXIE_H_ */