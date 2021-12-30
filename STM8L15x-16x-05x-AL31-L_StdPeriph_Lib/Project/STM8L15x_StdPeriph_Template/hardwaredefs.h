#ifndef HARDWAREDEFS_H_
#define HARDWAREDEFS_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/
#include "stm8l15x_gpio.h"
#include "stm8l15x_exti.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

/* Blinky LED address information */
#ifdef STM8_BASEBAND
#define LED_GPIO_PORT  GPIOA
#define LED_GPIO_PINS  GPIO_Pin_4
#else /* Watch */
#define LED_GPIO_PORT  GPIOD
#define LED_GPIO_PINS  GPIO_Pin_3
#endif /* STM8_BASEBAND */

/* Nixie tube power supply address information */
#ifdef STM8_BASEBAND
#define NIXIE_SUPPLY_PORT GPIOA
#define NIXIE_SUPPLY_PIN GPIO_Pin_5
#else /* Watch */
#define NIXIE_SUPPLY_PORT GPIOA
#define NIXIE_SUPPLY_PIN GPIO_Pin_4
#endif /* STM8_BASEBAND */

/* Nixie tube A address information */
#ifdef STM8_BASEBAND
/* Nixie GPIO output ports, multiple are needed to accomodate number of digits */
#define TUBE_A_PORT_0 GPIOE
#define TUBE_A_PORT_1 GPIOE
#define TUBE_A_PORT_2 GPIOE
#define TUBE_A_PORT_3 GPIOE
#define TUBE_A_PORT_4 GPIOE
#define TUBE_A_PORT_5 GPIOE
#define TUBE_A_PORT_6 GPIOE
#define TUBE_A_PORT_7 GPIOE
#define TUBE_A_PORT_8 GPIOA
#define TUBE_A_PORT_9 GPIOA
/* Nixie tube output pins */
#define DIGIT_A_0 GPIO_Pin_0
#define DIGIT_A_1 GPIO_Pin_1
#define DIGIT_A_2 GPIO_Pin_2
#define DIGIT_A_3 GPIO_Pin_3
#define DIGIT_A_4 GPIO_Pin_4
#define DIGIT_A_5 GPIO_Pin_5
#define DIGIT_A_6 GPIO_Pin_6
#define DIGIT_A_7 GPIO_Pin_7
#define DIGIT_A_8 GPIO_Pin_6
#define DIGIT_A_9 GPIO_Pin_7
#else /* Watch */
/* Nixie GPIO output ports, multiple are needed to accomodate number of digits */
#define TUBE_A_PORT_0 GPIOC
#define TUBE_A_PORT_1 GPIOC
#define TUBE_A_PORT_2 GPIOD
#define TUBE_A_PORT_3 GPIOC
#define TUBE_A_PORT_4 GPIOE
#define TUBE_A_PORT_5 GPIOE
#define TUBE_A_PORT_6 GPIOD
#define TUBE_A_PORT_7 GPIOC
#define TUBE_A_PORT_8 GPIOC
#define TUBE_A_PORT_9 GPIOC
/* Nixie tube output pins */
#define DIGIT_A_0 GPIO_Pin_4
#define DIGIT_A_1 GPIO_Pin_5
#define DIGIT_A_2 GPIO_Pin_7
#define DIGIT_A_3 GPIO_Pin_7
#define DIGIT_A_4 GPIO_Pin_6
#define DIGIT_A_5 GPIO_Pin_7
#define DIGIT_A_6 GPIO_Pin_6
#define DIGIT_A_7 GPIO_Pin_6
#define DIGIT_A_8 GPIO_Pin_0
#define DIGIT_A_9 GPIO_Pin_1
#endif /* STM8_BASEBAND */

/* Nixie tube B only supported on full watch */
#ifndef STM8_BASEBAND
/* Nixie GPIO output ports, multiple are needed to accomodate number of digits */
#define TUBE_B_PORT_0 GPIOB
#define TUBE_B_PORT_1 GPIOB
#define TUBE_B_PORT_2 GPIOD
#define TUBE_B_PORT_3 GPIOB
#define TUBE_B_PORT_4 GPIOB
#define TUBE_B_PORT_5 GPIOB
#define TUBE_B_PORT_6 GPIOD
#define TUBE_B_PORT_7 GPIOB
#define TUBE_B_PORT_8 GPIOF
#define TUBE_B_PORT_9 GPIOB
/* Nixie tube output pins */
#define DIGIT_B_0 GPIO_Pin_6
#define DIGIT_B_1 GPIO_Pin_5
#define DIGIT_B_2 GPIO_Pin_4
#define DIGIT_B_3 GPIO_Pin_3
#define DIGIT_B_4 GPIO_Pin_2
#define DIGIT_B_5 GPIO_Pin_1
#define DIGIT_B_6 GPIO_Pin_5
#define DIGIT_B_7 GPIO_Pin_4
#define DIGIT_B_8 GPIO_Pin_0
#define DIGIT_B_9 GPIO_Pin_7
#endif /* !STM8_BASEBAND */


/* Power switch/interrupt control */
#ifdef STM8_BASEBAND
/* Power switch */
#define POWER_SWITCH_PORT GPIOB
#define POWER_SWITCH_PIN_0 GPIO_Pin_0
#define POWER_SWITCH_PIN_1 GPIO_Pin_1
#define POWER_SWITCH_PIN_2 GPIO_Pin_2
#define POWER_SWITCH_INT_0 EXTI_Pin_0
#define POWER_SWITCH_INT_1 EXTI_Pin_1
#define POWER_SWITCH_INT_2 EXTI_Pin_2

/* Wake button */
#define WAKE_BUTTON_PORT GPIOB
#define WAKE_BUTTON_PIN GPIO_Pin_3
#define WAKE_BUTTON_INT EXTI_Pin_3
#else /* Watch */
/* Power switch */
#define POWER_SWITCH_PORT GPIOE
#define POWER_SWITCH_PIN_0 GPIO_Pin_3
#define POWER_SWITCH_PIN_1 GPIO_Pin_4
#define POWER_SWITCH_PIN_2 GPIO_Pin_5
#define POWER_SWITCH_INT_0 EXTI_Pin_3
#define POWER_SWITCH_INT_1 EXTI_Pin_4
#define POWER_SWITCH_INT_2 EXTI_Pin_5

/* Wake button */
#define WAKE_BUTTON_PORT GPIOB
#define WAKE_BUTTON_PIN GPIO_Pin_0
#define WAKE_BUTTON_INT EXTI_Pin_0
#endif /* STM8_BASEBAND */

#endif /* HARDWAREDEFS_H_ */