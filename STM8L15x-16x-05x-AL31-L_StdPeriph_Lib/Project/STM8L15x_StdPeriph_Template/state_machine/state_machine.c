/**
 * @file state_machine.h
 * @brief Implementation for watch state machine
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/
#include "state_machine.h"
#include "hardwaredefs.h"

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/
state_machine_t state_machine = {
  STATE_INIT,

  FALSE,

  {POWER_SWITCH_PORT, POWER_SWITCH_PIN_0, POWER_SWITCH_PIN_1, POWER_SWITCH_PIN_2, POWER_SWITCH_INT_0,
   POWER_SWITCH_INT_1, POWER_SWITCH_INT_2, WAKE_BUTTON_PORT, WAKE_BUTTON_PIN, WAKE_BUTTON_INT}
};

state_machine_req_t state_machine_request = {
  STATE_MESSAGE_NONE
};

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/
static void Delay(__IO uint16_t nCount);

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void sm_configure_interrupts(state_machine_t* sm)
{
  /* Initialize GPIO pins */
  GPIO_Init(sm->sm_interrupt.power_port, sm->sm_interrupt.power_pin_1, GPIO_Mode_In_PU_IT);
  GPIO_Init(sm->sm_interrupt.power_port, sm->sm_interrupt.power_pin_2, GPIO_Mode_In_PU_IT);
  GPIO_Init(sm->sm_interrupt.power_port, sm->sm_interrupt.power_pin_3, GPIO_Mode_In_PU_IT);
  //GPIO_Init(sm->sm_interrupt.wake_port, sm->sm_interrupt.wake_pin, GPIO_Mode_In_FL_IT);

  /* Set interrupts to trigger on falling edge */
  EXTI_SetPinSensitivity(sm->sm_interrupt.power_int_1, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(sm->sm_interrupt.power_int_2, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(sm->sm_interrupt.power_int_3, EXTI_Trigger_Falling);
  //EXTI_SetPinSensitivity(decode_interrupt_mask(sm->sm_interrupt.wake_pin), EXTI_Trigger_Falling);
}

void sm_execute_requests(state_machine_t* sm, state_machine_req_t* req)
{
  uint8_t time_buf[2] = {0};
  /* If there is no new state transition message, we can return/poll/enter lpm */
  if (req->message == STATE_MESSAGE_NONE)
  {
    return;
  }

  /* Block other interrupts from queueing new requests */
  sm->executing_state = TRUE;

  switch (req->message)
  {
    case STATE_MESSAGE_SET_SLEEP:
      sm->current_state = STATE_SLEEP;
      req->message = STATE_MESSAGE_NONE;
      break;
    case STATE_MESSAGE_POWER_DOWN:
      sm->current_state = STATE_POWEROFF;
      req->message = STATE_MESSAGE_NONE;
      break;
    case STATE_MESSAGE_PRINT_TIME:
      /* Only read time if device powered on */
      if (sm->current_state != STATE_POWEROFF)
      {
        #ifdef STM8_BASEBAND
        /* Read rtc data */
        ext_rtc_read(time_buf, RTC_PAY_READ_SIZE);
        /* Print RTC time */
        ext_rtc_print_val(time_buf[0], RTC_PRINT_SECONDS);
        ext_rtc_print_val(time_buf[1], RTC_PRINT_MINUTES);
        nixie_enable_psu(&shared_psu);
        nixie_digit_control(&tube_A, (uint8_t)(ext_rtc_decode(time_buf[0]) % 10), DIGIT_ON, &shared_psu);
        Delay(0xFFFF);
        Delay(0xFFFF);
        Delay(0xFFFF);
        nixie_disable_psu(&shared_psu);
        #endif /* STM8_BASEBAND */
        sm->current_state = STATE_SLEEP;
      }
      req->message = STATE_MESSAGE_NONE;
      break;
    case STATE_MESSAGE_SET_TIME:
      req->message = STATE_MESSAGE_NONE;
      break;
    default:
      req->message = STATE_MESSAGE_NONE;
      break;
  }

  sm->executing_state = FALSE;
}
/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/
/**
 * @brief  Inserts a delay time.
 * @param  nCount: specifies the delay time length.
 * @retval None
 */
static void Delay(__IO uint16_t nCount)
{
  /* Decrement nCount value */
	while (nCount != 0)
  {
    nCount--;
  }
}
