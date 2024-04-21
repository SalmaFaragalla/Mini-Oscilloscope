/**
 ******************************************************************************
 * @file    TIM_interface.h
 * @author  Salma Faragalla
 * @ brief  Header file of TIM module.
 ******************************************************************************
 */
#ifndef TIM_TIM_INTERFACE_H_
#define TIM_TIM_INTERFACE_H_

#include "STM32F103.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @typedef TIM_CH_t
 * @brief Enumeration of timer channels.
 */
typedef enum {
	TIM_CH1 = 0,
	TIM_CH2,
	TIM_CH3,
	TIM_CH4,
}TIM_CH_t;

/**
 * @typedef TIM_IC_Edge_t
 * @brief Enumeration of timer capture edges.
 */
typedef enum {
	TIM_IC_RISING_EDGE=0,
	TIM_IC_FALLING_EDGE
}TIM_IC_Edge_t;
/**
 * @typedef TIM_INT_Status_t
 * @brief Enumeration for Timer Interrupt Enable/Disable.
 */
typedef enum {
	TIM_INT_DISABLE = 0,
	TIM_INT_ENABLE
}TIM_INT_Status_t;

/**
 * @brief  Initializes the clock for the specified timer peripheral.
 * @param  TIMx : Pointer to the timer peripheral (TIM1, TIM2, or TIM3).
 * @retval None
 */

/* Exported functions --------------------------------------------------------*/

void TIM_Init(volatile TIM_TypeDef *TIMx);

/**
 * @brief  Starts a PWM signal on the specified timer channel with the given duty cycle and frequency.
 * @param  TIMx : Pointer to the timer peripheral (TIM1, TIM2, or TIM3).
 * @param  TIM_CHx : Timer channel (TIM_CH1, TIM_CH2, TIM_CH3, or TIM_CH4).
 * @param  dutyCycle :  Desired dutly cycle (0-100)
 * @param  frequency : Desired frequency of the PWM signal in Hz (123 Hz - 80000 Hz)
 * @retval None
 */
void TIM_PWM_Start(volatile TIM_TypeDef* TIMx ,TIM_CH_t TIM_CHx , u32 dutyCycle , u32 frequency);

/**
 * @brief  Starts Input Capture (IC) mode on the specified timer channel with the given configuration.
 * @param  TIMx : Pointer to the timer peripheral (TIM1, TIM2, or TIM3).
 * @param  TIM_CHx : Timer channel (TIM_CH1, TIM_CH2, TIM_CH3, or TIM_CH4).
 * @param  CCS_Direction : Capture/Compare selection from @defgroup CCS_DIRECTION
 * @param  TIM_IC_Edge : Selection of edge for capture (TIM_IC_RISING_EDGE or TIM_IC_FALLING_EDGE)
 * @param  TIM_INT_Status: Enable/disable timer capture interrupt (TIM_INT_ENABLE or TIM_INT_DISABLE).
 * @retval None
 */
void TIM_IC_Start (volatile TIM_TypeDef* TIMx,TIM_CH_t TIM_CHx,u8 CCS_Direction , TIM_IC_Edge_t TIM_IC_Edge,TIM_INT_Status_t TIM_INT_Status);

/**
 * @brief  Enables the interrupt for the specified timer's capture/compare events.
 * @param  TIMx : Pointer to the timer peripheral (TIM1, TIM2, or TIM3).
 * @retval None
 */
void TIM_IC_INT_Enable (volatile TIM_TypeDef* TIMx);
/**
 * @brief  Sets the callback function for the TIM1 update event interrupt
 * @param  functionPtr :  Pointer to the callback function.
 * @retval None
 */

/* Callback functions --------------------------------------------------------*/
void TIM1_UP_SetCallback(void ( *functionPtr ) ( void ));
/**
 * @brief  Sets the callback function for the TIM1 trigger and communication interrupts.
 * @param  functionPtr :  Pointer to the callback function.
 * @retval None
 */
void TIM1_TRG_COM_SetCallback(void ( *functionPtr ) ( void ));
/**
 * @brief  Sets the callback function for the capture/compare interrupt
 * @param  functionPtr :  Pointer to the callback function.
 * @retval None
 */
void TIM1_CC_SetCallback(void ( *functionPtr ) ( void ));

/**
 * @brief  Sets the callback function for the TIM2 interrupt.
 * @param  functionPtr :  Pointer to the callback function.
 * @retval None
 */
void TIM2_SetCallback(void ( *functionPtr ) ( void ));

/**
 * @brief  Sets the callback function for the TIM3 interrupt.
 * @param  functionPtr :  Pointer to the callback function.
 * @retval None
 */
void TIM3_SetCallback(void ( *functionPtr ) ( void ));

#endif /* TIM_TIM_INTERFACE_H_ */
