/*
 * TIM_interface.h
 */

#ifndef TIM_TIM_INTERFACE_H_
#define TIM_TIM_INTERFACE_H_

#include "STM32F103.h"

typedef enum {
	TIM_CH1 = 0,
	TIM_CH2,
	TIM_CH3,
	TIM_CH4,
}TIM_CH_t;

typedef enum {
	TIM_IC_RISING_EDGE=0,
	TIM_IC_FALLING_EDGE
}TIM_IC_Edge_t;

void TIM_Init(volatile TIM_TypeDef *TIMx);

void TIM_PWM_Start(volatile TIM_TypeDef* TIMx ,TIM_CH_t TIM_CHx , u32 dutyCycle , u32 frequency);
void TIM_IC_Start (volatile TIM_TypeDef* TIMx,TIM_CH_t TIM_CHx,u8 CCS_Direction , TIM_IC_Edge_t TIM_IC_Edge);
void TIM_IC_INT_Enable (volatile TIM_TypeDef* TIMx);

void TIM1_UP_SetCallback(void ( *functionPtr ) ( void ));
void TIM1_TRG_COM_SetCallback(void ( *functionPtr ) ( void ));
void TIM1_CC_SetCallback(void ( *functionPtr ) ( void ));
void TIM2_SetCallback(void ( *functionPtr ) ( void ));
void TIM3_SetCallback(void ( *functionPtr ) ( void ));

#endif /* TIM_TIM_INTERFACE_H_ */
