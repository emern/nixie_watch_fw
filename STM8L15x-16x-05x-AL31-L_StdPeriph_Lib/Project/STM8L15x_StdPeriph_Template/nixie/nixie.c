/**
 * @file nixie.c
 * @brief Contains implementation for nixie tube driver
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/
#include "nixie.h"
#include "hardwaredefs.h"

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

nixie_psu_t shared_psu = {
    FALSE,

    NIXIE_SUPPLY_PORT,

    NIXIE_SUPPLY_PIN
};

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/
nixie_tube_t tube_A = {
    {{DIGIT_OFF, TUBE_A_PORT_0, DIGIT_A_0}, {DIGIT_OFF, TUBE_A_PORT_1, DIGIT_A_1}, {DIGIT_OFF, TUBE_A_PORT_2, DIGIT_A_2},
     {DIGIT_OFF, TUBE_A_PORT_3, DIGIT_A_3}, {DIGIT_OFF, TUBE_A_PORT_4, DIGIT_A_4}, {DIGIT_OFF, TUBE_A_PORT_5, DIGIT_A_5},
     {DIGIT_OFF, TUBE_A_PORT_6, DIGIT_A_6}, {DIGIT_OFF, TUBE_A_PORT_7, DIGIT_A_7}, {DIGIT_OFF, TUBE_A_PORT_8, DIGIT_A_8},
     {DIGIT_OFF, TUBE_A_PORT_9, DIGIT_A_9}}
};

#ifndef STM8_BASEBAND
nixie_tube_t tube_B = {
    {{DIGIT_OFF, TUBE_B_PORT_0, DIGIT_B_0}, {DIGIT_OFF, TUBE_B_PORT_1, DIGIT_B_1}, {DIGIT_OFF, TUBE_B_PORT_2, DIGIT_B_2},
     {DIGIT_OFF, TUBE_B_PORT_3, DIGIT_B_3}, {DIGIT_OFF, TUBE_B_PORT_4, DIGIT_B_4}, {DIGIT_OFF, TUBE_B_PORT_5, DIGIT_B_5},
     {DIGIT_OFF, TUBE_B_PORT_6, DIGIT_A_6}, {DIGIT_OFF, TUBE_B_PORT_7, DIGIT_A_7}, {DIGIT_OFF, TUBE_B_PORT_8, DIGIT_B_8},
     {DIGIT_OFF, TUBE_B_PORT_9, DIGIT_B_9}}
};
#endif /* STM8_BASEBAND */

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

/**
 * @brief Initialize nixie tube GPIO pins, should be called once at startup
 *
 * @param tube: Nixie tube to initialize
 * @param psu: Power supply addressing and state information
 */
void nixie_init_pins(nixie_tube_t* tube, nixie_psu_t* psu)
{
    uint8_t i;

    /* Initialize PSU pin first (if multiple nixies share the same PSU, this should not cause any issues) */
    GPIO_Init(psu->psu_port, psu->psu_pin, GPIO_Mode_Out_PP_Low_Fast);
    /* Disable PSU by default */
    GPIO_SetBits(psu->psu_port, psu->psu_pin);
    psu->psu_enabled = FALSE;

    /* Initialize all nixie digit pins */
    for (i=0; i<NUM_NIXIE_DIGITS; i++)
    {
        GPIO_Init(tube->digits[i].gpio_port, tube->digits[i].gpio_pin, GPIO_Mode_Out_PP_Low_Fast);
        /* OFF state is voltage high at MOSFET driver gate */
        GPIO_ResetBits(tube->digits[i].gpio_port, tube->digits[i].gpio_pin);
        tube->digits[i].curr_state = DIGIT_OFF;
    }
}

/**
 * @brief Enable nixie tube PSU
 *
 * @param psu: Power supply addressing and state information
 */
void nixie_enable_psu(nixie_psu_t* psu)
{
    GPIO_ResetBits(psu->psu_port, psu->psu_pin);
    psu->psu_enabled = TRUE;
}

/**
 * @brief Disable nixie tube PSU
 *
 * @param psu: Power supply addressing and state information
 */
void nixie_disable_psu(nixie_psu_t* psu)
{
    GPIO_SetBits(psu->psu_port, psu->psu_pin);
    psu->psu_enabled = FALSE;
}

/**
 * @brief Control a nixie tube digit (on or off)
 *
 * @param tube: Nixie tube to control
 * @param digit: Digit number to control (0-9)
 * @param state: State to change to (DIGIT_ON or DIGIT_OFF)
 * @param psu: Power supply addressing and state information
 * @return nixie_error_t
 */
nixie_error_t nixie_digit_control(nixie_tube_t* tube, uint8_t digit, nixie_digit_state_t state, nixie_psu_t* psu)
{
    uint8_t i;

    /* No modifying nixie states if power supply is turned off */
    if (psu->psu_enabled == FALSE)
    {
        return NIXIE_PSU_DISABLED;
    }

    switch (state)
    {
        case DIGIT_ON:
            for (i=0; i<NUM_NIXIE_DIGITS; i++)
            {
                /* Turn off any previously on nixie digits, only one should be on at once */
                if (tube->digits[i].curr_state == DIGIT_ON)
                {
                    GPIO_ResetBits(tube->digits[i].gpio_port, tube->digits[i].gpio_pin);
                    tube->digits[i].curr_state = DIGIT_OFF;
                }
            }
            /* Turn on new digit */
            GPIO_SetBits(tube->digits[digit].gpio_port, tube->digits[digit].gpio_pin);
            tube->digits[digit].curr_state = DIGIT_ON;
            break;

        case DIGIT_OFF:
            GPIO_ResetBits(tube->digits[digit].gpio_port, tube->digits[digit].gpio_pin);
            tube->digits[digit].curr_state = DIGIT_OFF;
            break;

        default:
            return NIXIE_ERROR;
    }
    return NIXIE_SUCCESS;
}
