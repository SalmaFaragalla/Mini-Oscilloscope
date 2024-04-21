/**
 ******************************************************************************
 * @file    APP_interface.h
 * @author  Salma Faragalla
 * @brief   Header file of APP module.
 ******************************************************************************
 */
#ifndef APP_INTERFACE_H_
#define APP_INTERFACE_H_

#include "STD_TYPES.h"

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initializes the GLCD and timers used for PWM and input capture.
 * @param  None
 * @retval None
 */
void APP_Init(void);
/**
 * @brief Starts the input capture timer for frequency and duty cycle calculations.
 * @param  None
 * @retval None
 */
void APP_IC_Start(void);
/**
 * @brief Starts the timer in PWM output mode.
 * @param  None
 * @retval None
 */
void APP_PWM_Start(u32 dutyCycle, u32 frequency);
/**
 * @brief  Prints on GLCD the initial frequency, duty, period, and draws initial PWM signal
 * @param  None
 * @retval None
 */
void APP_GLCD_Print_Init();
/**
 * @brief  Updates GLCD display with new frequency, duty, period, and PWM signal if they have changed.
 * @param  None
 * @retval None
 */
void APP_GLCD_Update();

#endif /* APP_INTERFACE_H_ */
