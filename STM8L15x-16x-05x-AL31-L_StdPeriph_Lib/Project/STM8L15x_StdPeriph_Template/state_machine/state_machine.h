/**
 * @file state_machine.h
 * @brief Function prototypes, defines and types for system state machine
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/
#include "stm8l15x_gpio.h"
#include "ext_rtc.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

/**
 * @brief State machine active state type
 */
typedef enum
{
  STATE_POWEROFF, /* Poweroff state */

  STATE_INIT, /* Initialization state */

  STATE_SLEEP, /* Put processor into HALT mode and wait for further interrupts */

  STATE_PRINT /* Print time via nixie tubes */

} state_t;

/**
 * @brief State machine message type
 */
typedef enum
{
  STATE_MESSAGE_NONE, /* No new message, for polling/error handling purposes */

  STATE_MESSAGE_SET_SLEEP, /* Set watch to sleep/idle mode, ready to accept print requests */

  STATE_MESSAGE_POWER_DOWN, /**/

  STATE_MESSAGE_PRINT_TIME,

  STATE_MESSAGE_SET_TIME

} state_message_t;

/**
 * @brief State machine interrupt driver pins/ports
 */
typedef struct
{
  GPIO_TypeDef* power_port;

  uint8_t power_pin_1;

  uint8_t power_pin_2;

  uint8_t power_pin_3;

  EXTI_Pin_TypeDef power_int_1;

  EXTI_Pin_TypeDef power_int_2;

  EXTI_Pin_TypeDef power_int_3;

  GPIO_TypeDef* wake_port;

  uint8_t wake_pin;

  EXTI_Pin_TypeDef wake_int;

} state_machine_driver_t;

/**
 * @brief State machine configuration/current status
 */
typedef struct
{
  state_t current_state;

  bool executing_state;

  state_machine_driver_t sm_interrupt;

} state_machine_t;

/**
 * @brief State machine message struct, TODO: determine if more complicated queue setup is needed here
 */
typedef struct
{
  state_message_t message;

} state_machine_req_t;

/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/
extern state_machine_t state_machine;
extern state_machine_req_t state_machine_request;

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/
void sm_execute_requests(state_machine_t* sm, state_machine_req_t* req);
void sm_configure_interrupts(state_machine_t* sm);



#endif /* STATE_MACHINE_H_ */