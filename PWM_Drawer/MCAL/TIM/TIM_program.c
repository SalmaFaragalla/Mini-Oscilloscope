/*
 * TIM_program.c
 */

/* Includes -------------------------------------------------------------------*/
#include "BIT_MATH.h"

#include "../GPIO/GPIO_interface.h"
#include "../NVIC/NVIC_interface.h"

#include "TIM_interface.h"

/* Defines -------------------------------------------------------------------*/
#define TIM_PERIOD (100UL)

/* Private Variables -------------------------------------------------------------------*/
static void (*TIM1_UP_Callback_Ptr)(void);
static void (*TIM1_TRG_COM_Callback_Ptr)(void);
static void (*TIM1_CC_Callback_Ptr)(void);
static void (*TIM2_Callback_Ptr)(void);
static void (*TIM3_Callback_Ptr)(void);


/* Public Functions -------------------------------------------------------------------*/

void TIM_Init(volatile TIM_TypeDef *TIMx)
{
	RCC_AFIO_CLK_EN();
	if (TIMx == TIM1)
	{
		RCC_TIM1_CLK_EN();
		RCC_GPIOA_CLK_EN();
	}

	else if (TIMx == TIM2)
	{
		RCC_TIM2_CLK_EN();
		RCC_GPIOA_CLK_EN();
	}

	else if (TIMx == TIM3)
	{
		RCC_TIM3_CLK_EN();
		RCC_GPIOA_CLK_EN();
		RCC_GPIOB_CLK_EN();
	}
}

// Frequency must be 123<freq<80000
void TIM_PWM_Start(volatile TIM_TypeDef *TIMx, TIM_CH_t TIM_CHx, u32 dutyCycle, u32 frequency)
{
	/* Individual channel initialization*/
	switch (TIM_CHx)
	{
	case TIM_CH1:
		TIMx->CCR1 = dutyCycle;
		TIMx->CCMR1 |= OCM_PWM2 << CCMR1_OC1M; // Select PWM Mode
		SET_BIT(TIMx->CCMR1, CCMR1_OC1PE);	   // Enable the corresponding preload register
		SET_BIT(TIMx->CCER, CCER_CC1E);
		break;

	case TIM_CH2:
		TIMx->CCR2 = dutyCycle;
		TIMx->CCMR1 |= OCM_PWM2 << CCMR1_OC2M; // Select PWM Mode
		SET_BIT(TIMx->CCMR1, CCMR1_OC2PE);	   // Enable the corresponding preload register
		SET_BIT(TIMx->CCER, CCER_CC2E);
		break;

	case TIM_CH3:
		TIMx->CCR3 = dutyCycle;
		TIMx->CCMR2 |= OCM_PWM2 << CCMR2_OC3M; // Select PWM Mode
		SET_BIT(TIMx->CCMR2, CCMR2_OC3PE);	   // Enable the corresponding preload register
		SET_BIT(TIMx->CCER, CCER_CC3E);
		break;

	case TIM_CH4:
		TIMx->CCR4 = dutyCycle;
		TIMx->CCMR2 |= OCM_PWM2 << CCMR2_OC4M; // Select PWM Mode
		SET_BIT(TIMx->CCMR2, CCMR2_OC4PE);	   // Enable the corresponding preload register
		SET_BIT(TIMx->CCER, CCER_CC4E);
		break;
	}

	/* Common initialization for all timer n channels */
	TIMx->ARR = TIM_PERIOD;
	TIMx->PSC = (TIM_CLK / (TIMx->ARR * frequency)) - 1;

	if (TIMx == TIM1)
	{
		SET_BIT(TIMx->BDTR, BDTR_MOE);
	}

	/*As the preload registers are transferred to the shadow registers only when an update event
	occurs, before starting the counter, the user must initialize all the registers by setting the UG
	bit in the TIMx_EGR register
	*/
	SET_BIT(TIMx->EGR, EGR_UG);
	SET_BIT(TIMx->CR1, CR1_ARPE); //  Enable the auto-reload preload register
	SET_BIT(TIMx->CR1, CR1_CEN);
}

void TIM_IC_Start(volatile TIM_TypeDef *TIMx, TIM_CH_t TIM_CHx, u8 CCS_Direction, TIM_IC_Edge_t TIM_IC_Edge)
{

	switch (TIM_CHx)
	{
	case TIM_CH1:
		TIMx->CCMR1 |= (CCS_Direction << CCMR1_CC1S);
		if (TIM_IC_Edge == TIM_IC_RISING_EDGE)
		{
			CLR_BIT(TIMx->CCER, CCER_CC1P);
		}
		else if (TIM_IC_Edge == TIM_IC_FALLING_EDGE)
		{
			SET_BIT(TIMx->CCER, CCER_CC1P);
		}
		SET_BIT(TIMx->DIER, DIER_CC1IE);
		SET_BIT(TIMx->CCER, CCER_CC1E);
		break;

	case TIM_CH2:
		TIMx->CCMR1 |= (CCS_Direction << CCMR1_CC2S);
		if (TIM_IC_Edge == TIM_IC_RISING_EDGE)
		{
			CLR_BIT(TIMx->CCER, CCER_CC2P);
		}
		else if (TIM_IC_Edge == TIM_IC_FALLING_EDGE)
		{
			SET_BIT(TIMx->CCER, CCER_CC2P);
		}
		//SET_BIT(TIMx->DIER, DIER_CC2IE);
		SET_BIT(TIMx->CCER, CCER_CC2E);
		break;

	case TIM_CH3:
		TIMx->CCMR2 |= CCS_Direction << CCMR2_CC3S;
		if (TIM_IC_Edge == TIM_IC_RISING_EDGE)
		{
			CLR_BIT(TIMx->CCER, CCER_CC3P);
		}
		else if (TIM_IC_Edge == TIM_IC_FALLING_EDGE)
		{
			SET_BIT(TIMx->CCER, CCER_CC3P);
		}
		SET_BIT(TIMx->DIER, DIER_CC3IE);
		SET_BIT(TIMx->CCER, CCER_CC3E);
		break;

	case TIM_CH4:
		TIMx->CCMR2 |= CCS_Direction << CCMR2_CC4S;
		if (TIM_IC_Edge == TIM_IC_RISING_EDGE)
		{
			CLR_BIT(TIMx->CCER, CCER_CC4P);
		}
		else if (TIM_IC_Edge == TIM_IC_FALLING_EDGE)
		{
			SET_BIT(TIMx->CCER, CCER_CC4P);
		}
		SET_BIT(TIMx->DIER, DIER_CC4IE);
		SET_BIT(TIMx->CCER, CCER_CC4E);
		break;
	}
	TIMx->ARR = TIM_MAX_PERIOD;
	SET_BIT(TIMx->CR1, CR1_CEN);
}

void TIM_IC_INT_Enable(volatile TIM_TypeDef *TIMx)
{
	if (TIMx == TIM1)
	{
		NVIC_EnableIRQ(TIM1_CC_IRQn);
	}
	else if (TIMx == TIM2)
	{
		NVIC_EnableIRQ(TIM2_IRQn);
	}
	else if (TIMx == TIM3)
	{
		NVIC_EnableIRQ(TIM3_IRQn);
	}
}

void TIM1_UP_SetCallback(void (*functionPtr)(void))
{
	TIM1_UP_Callback_Ptr = functionPtr;
}
void TIM1_TRG_COM_SetCallback(void (*functionPtr)(void))
{
	TIM1_TRG_COM_Callback_Ptr = functionPtr;
}

void TIM1_CC_SetCallback(void (*functionPtr)(void))
{
	TIM1_CC_Callback_Ptr = functionPtr;
}
void TIM2_SetCallback(void (*functionPtr)(void))
{
	TIM2_Callback_Ptr = functionPtr;
}
void TIM3_SetCallback(void (*functionPtr)(void))
{
	TIM3_Callback_Ptr = functionPtr;
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM1_UP_Callback_Ptr != 0)
	{
		TIM1_UP_Callback_Ptr();
	}
}
void TIM1_TRG_COM_IRQHandler(void)
{
	if (TIM1_TRG_COM_Callback_Ptr != 0)
	{
		TIM1_TRG_COM_Callback_Ptr();
	}
}
void TIM1_CC_IRQHandler(void)
{
	if (TIM1_CC_Callback_Ptr != 0)
	{
		TIM1_CC_Callback_Ptr();
	}
}
void TIM2_IRQHandler(void)
{
	if (TIM3_Callback_Ptr != 0)
	{
		TIM2_Callback_Ptr();
	}
}

void TIM3_IRQHandler(void)
{

	if (TIM3_Callback_Ptr != 0)
	{
		TIM3_Callback_Ptr();
	}

}
