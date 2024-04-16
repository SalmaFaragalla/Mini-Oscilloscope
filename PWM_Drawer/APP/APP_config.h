/*
 * APP_config.h
 */

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#include "STM32F103.h"
#include "../HAL/GLCD/GLCD_interface.h"

/*Timers configurations-----------------------------------------------------*/
#define APP_TIM_PWM_TIMx (TIM2)
#define APP_TIM_PWM_CHx (TIM_CH3)

#define APP_TIM_IC1_TIMx (TIM3)
#define APP_TIM_IC1_CHx (TIM_CH1)

#define APP_TIM_IC2_TIMx (TIM1)
#define APP_TIM_IC2_CHx (TIM_CH2)

/*GLCD configurations-----------------------------------------------------*/
#define APP_GLCD_LOW_LINE (GLCD_LINE_7)
#define APP_GLCD_HIGH_LINE (GLCD_LINE_5)

#define APP_GLCD_CYCLE_1_START_X (7UL)
#define APP_GLCD_CYCLE_2_START_X (APP_GLCD_CYCLE_1_START_X + 60UL)
#define APP_GLCD_CYCLE_2_END_X (127UL)

# define APP_GLCD_FREQ_LINE (GLCD_LINE_0)
# define APP_GLCD_DUTY_LINE (GLCD_LINE_1)
# define APP_GLCD_PERIOD_LINE (GLCD_LINE_2)

#endif /* APP_CONFIG_H_ */
