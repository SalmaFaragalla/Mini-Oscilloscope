/**
 ******************************************************************************
 * @file    TIM_interface.h
 * @author  Salma Faragalla
 * @ brief  TIM module driver
 ******************************************************************************
 */
/* Includes -------------------------------------------------------------------*/
#include "BIT_MATH.h"

#include "../GPIO/GPIO_interface.h"
#include "../NVIC/NVIC_interface.h"

#include "TIM_interface.h"

/* Defines -------------------------------------------------------------------*/

/* Private Variables -------------------------------------------------------------------*/
static void (*TIM1_UP_Callback_Ptr)(void);
static void (*TIM1_TRG_COM_Callback_Ptr)(void);
static void (*TIM1_CC_Callback_Ptr)(void);
static void (*TIM2_Callback_Ptr)(void);
static void (*TIM3_Callback_Ptr)(void);

/*Private Functions prototypes -------------------------------------------------*/
/**
 * @brief  Initializes the pin configuration for a specific timer channel.
 * @param  TIMx : Pointer to the timer peripheral (TIM1, TIM2, or TIM3).
 * @param  TIM_CHx : Timer channel (TIM_CH1, TIM_CH2, TIM_CH3, or TIM_CH4).
 * @param  GPIO_Mode : Mode to configure the GPIO pins
 * @retval None
 */
static void TIM_Pin_Init(volatile TIM_TypeDef *TIMx, TIM_CH_t TIM_CHx,GPIO_Mode_t GPIO_Mode);

/* Private Functions -------------------------------------------------------------------*/
/**
 * @brief  Initializes the pin configuration for a specific timer channel.
 * @param  TIMx : Pointer to the timer peripheral (TIM1, TIM2, or TIM3).
 * @param  TIM_CHx : Timer channel (TIM_CH1, TIM_CH2, TIM_CH3, or TIM_CH4).
 * @param  GPIO_Mode : Mode to configure the GPIO pins
 * @retval None
 */
static void TIM_Pin_Init(volatile TIM_TypeDef *TIMx, TIM_CH_t TIM_CHx,GPIO_Mode_t GPIO_Mode)
{
	if (TIMx == TIM1)
	{
		switch(TIM_CHx)
		{
		case TIM_CH1:
			GPIO_SetPinDirSpeed(GPIOA, GPIO_PIN8, GPIO_Mode);
			break;
		case TIM_CH2:
			GPIO_SetPinDirSpeed(GPIOA, GPIO_PIN9, GPIO_Mode);
			break;
		case TIM_CH3:
			GPIO_SetPinDirSpeed(GPIOA, GPIO_PIN10, GPIO_Mode);
			break;
		case TIM_CH4:
			GPIO_SetPinDirSpeed(GPIOA, GPIO_PIN11, GPIO_Mode);
			break;
		}
	}
	else if (TIMx == TIM2)
	{
		switch(TIM_CHx)
		{
		case TIM_CH1:
			GPIO_SetPinDirSpeed(GPIOA, GPIO_PIN0, GPIO_Mode);
			break;
		case TIM_CH2:
			GPIO_SetPinDirSpeed(GPIOA, GPIO_PIN1, GPIO_Mode);
			break;
		case TIM_CH3:
			GPIO_SetPinDirSpeed(GPIOA, GPIO_PIN2, GPIO_Mode);
			break;
		case TIM_CH4:
			GPIO_SetPinDirSpeed(GPIOA, GPIO_PIN3, GPIO_Mode);
			break;
		}
	}
	else if (TIMx == TIM3)
	{
		switch(TIM_CHx)
		{
		case TIM_CH1:
			GPIO_SetPinDirSpeed(GPIOA, GPIO_PIN6, GPIO_Mode);
			break;
		case TIM_CH2:
			GPIO_SetPinDirSpeed(GPIOA, GPIO_PIN7, GPIO_Mode);
			break;
		case TIM_CH3:
			GPIO_SetPinDirSpeed(GPIOB, GPIO_PIN0, GPIO_Mode);
			break;
		case TIM_CH4:
			GPIO_SetPinDirSpeed(GPIOB, GPIO_PIN1, GPIO_Mode);
			break;
		}
	}

}


/* Public Functions -------------------------------------------------------------------*/
/**
 * @brief  Initializes the clock for the specified timer peripheral.
 * @param  TIMx : Pointer to the timer peripheral (TIM1, TIM2, or TIM3).
 * @retval None
 */
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

/**
 * @brief  Starts a PWM signal on the specified timer channel with the given duty cycle and frequency.
 * @param  TIMx : Pointer to the timer peripheral (TIM1, TIM2, or TIM3).
 * @param  TIM_CHx : Timer channel (TIM_CH1, TIM_CH2, TIM_CH3, or TIM_CH4).
 * @param  dutyCycle :  Desired dutly cycle (0-100)
 * @param  frequency : Desired frequency of the PWM signal in Hz (123 Hz - 80000 Hz)
 * @retval None
 */
void TIM_PWM_Start(volatile TIM_TypeDef *TIMx, TIM_CH_t TIM_CHx, u32 dutyCycle, u32 frequency)
{
	/* Pin initialization for timer channel */
	TIM_Pin_Init(TIMx, TIM_CHx, GPIO_OUTPUT_AF_PP_2MHZ);

	/* Individual channel initialization*/
	switch (TIM_CHx)
	{
	case TIM_CH1:
		TIMx->CCR1 = dutyCycle;
		TIMx->CCMR1 |= OCM_PWM2 << CCMR1_OC1M; // Select PWM Mode
		SET_BIT(TIMx->CCMR1, CCMR1_OC1PE);	   // Enable the corresponding preload register
		SET_BIT(TIMx->CCER, CCER_CC1E);        // Enable capture/compare channel
		break;

	case TIM_CH2:
		TIMx->CCR2 = dutyCycle;
		TIMx->CCMR1 |= OCM_PWM2 << CCMR1_OC2M; // Select PWM Mode
		SET_BIT(TIMx->CCMR1, CCMR1_OC2PE);	   // Enable the corresponding preload register
		SET_BIT(TIMx->CCER, CCER_CC2E);		   // Enable capture/compare channel
		break;

	case TIM_CH3:
		TIMx->CCR3 = dutyCycle;
		TIMx->CCMR2 |= OCM_PWM2 << CCMR2_OC3M; // Select PWM Mode
		SET_BIT(TIMx->CCMR2, CCMR2_OC3PE);	   // Enable the corresponding preload register
		SET_BIT(TIMx->CCER, CCER_CC3E);        // Enable capture from the counter into the capture register
		break;

	case TIM_CH4:
		TIMx->CCR4 = dutyCycle;
		TIMx->CCMR2 |= OCM_PWM2 << CCMR2_OC4M; // Select PWM Mode
		SET_BIT(TIMx->CCMR2, CCMR2_OC4PE);	   // Enable the corresponding preload register
		SET_BIT(TIMx->CCER, CCER_CC4E);        // Enable capture/compare channel
		break;
	}

	/* Common initialization for all timer channels */
	TIMx->ARR = 100UL;  // Set ARR to 100 to allow direct mapping of the duty cycle value to the range of 0 to 100
	TIMx->PSC = (TIM_CLK / (TIMx->ARR * frequency)) - 1;

	if (TIMx == TIM1)
	{
		SET_BIT(TIMx->BDTR, BDTR_MOE); //  Main output enable for TIM1 only
	}

	SET_BIT(TIMx->EGR, EGR_UG);   // Generate an update event
	SET_BIT(TIMx->CR1, CR1_ARPE); //  Enable the auto-reload preload register
	SET_BIT(TIMx->CR1, CR1_CEN);  // Enable the Timer/Counter
}

/**
 * @brief  Starts Input Capture (IC) mode on the specified timer channel with the given configuration.
 * @param  TIMx : Pointer to the timer peripheral (TIM1, TIM2, or TIM3).
 * @param  TIM_CHx : Timer channel (TIM_CH1, TIM_CH2, TIM_CH3, or TIM_CH4).
 * @param  CCS_Direction : Capture/Compare selection from @defgroup CCS_DIRECTION
 * @param  TIM_IC_Edge : Selection of edge for capture (TIM_IC_RISING_EDGE or TIM_IC_FALLING_EDGE)
 * @param  TIM_INT_Status: Enable/disable timer capture interrupt (TIM_INT_ENABLE or TIM_INT_DISABLE).
 * @retval None
 */
void TIM_IC_Start(volatile TIM_TypeDef *TIMx, TIM_CH_t TIM_CHx, u8 CCS_Direction, TIM_IC_Edge_t TIM_IC_Edge , TIM_INT_Status_t TIM_INT_Status)
{
	/* Pin initialization for timer channel */
	TIM_Pin_Init(TIMx, TIM_CHx, GPIO_INPUT_FLOATING);

	/* Individual channel initialization*/
	switch (TIM_CHx)
	{
	case TIM_CH1:
		TIMx->CCMR1 |= (CCS_Direction << CCMR1_CC1S); // Select the active input

		// Select the active polarity
		if (TIM_IC_Edge == TIM_IC_RISING_EDGE)
		{
			CLR_BIT(TIMx->CCER, CCER_CC1P);
		}
		else if (TIM_IC_Edge == TIM_IC_FALLING_EDGE)
		{
			SET_BIT(TIMx->CCER, CCER_CC1P);
		}

		// Enable or disable Interrupt
		if (TIM_INT_Status == TIM_INT_ENABLE)
		{
			SET_BIT(TIMx->DIER, DIER_CC1IE);
		}
		else if (TIM_INT_Status == TIM_INT_DISABLE)
		{
			CLR_BIT(TIMx->DIER, DIER_CC1IE);
		}
		SET_BIT(TIMx->CCER, CCER_CC1E);  // Enable capture/compare channel
		break;

	case TIM_CH2:
		TIMx->CCMR1 |= (CCS_Direction << CCMR1_CC2S); // Select the active input

		// Select the active polarity
		if (TIM_IC_Edge == TIM_IC_RISING_EDGE)
		{
			CLR_BIT(TIMx->CCER, CCER_CC2P);
		}
		else if (TIM_IC_Edge == TIM_IC_FALLING_EDGE)
		{
			SET_BIT(TIMx->CCER, CCER_CC2P);
		}

		// Enable or disable Interrupt
		if (TIM_INT_Status == TIM_INT_ENABLE)
		{
			SET_BIT(TIMx->DIER, DIER_CC2IE);
		}
		else if (TIM_INT_Status == TIM_INT_DISABLE)
		{
			CLR_BIT(TIMx->DIER, DIER_CC2IE);
		}

		SET_BIT(TIMx->CCER, CCER_CC2E);  // Enable capture/compare channel
		break;

	case TIM_CH3:
		TIMx->CCMR2 |= CCS_Direction << CCMR2_CC3S; // Select the active input

		// Select the active polarity
		if (TIM_IC_Edge == TIM_IC_RISING_EDGE)
		{
			CLR_BIT(TIMx->CCER, CCER_CC3P);
		}
		else if (TIM_IC_Edge == TIM_IC_FALLING_EDGE)
		{
			SET_BIT(TIMx->CCER, CCER_CC3P);
		}

		// Enable or disable Interrupt
		if (TIM_INT_Status == TIM_INT_ENABLE)
		{
			SET_BIT(TIMx->DIER, DIER_CC3IE);
		}
		else if (TIM_INT_Status == TIM_INT_DISABLE)
		{
			CLR_BIT(TIMx->DIER, DIER_CC3IE);
		}

		SET_BIT(TIMx->CCER, CCER_CC3E);  // Enable capture/compare channel
		break;

	case TIM_CH4:
		TIMx->CCMR2 |= CCS_Direction << CCMR2_CC4S; // Select the active input

		// Select the active polarity
		if (TIM_IC_Edge == TIM_IC_RISING_EDGE)
		{
			CLR_BIT(TIMx->CCER, CCER_CC4P);
		}
		else if (TIM_IC_Edge == TIM_IC_FALLING_EDGE)
		{
			SET_BIT(TIMx->CCER, CCER_CC4P);
		}

		// Enable or disable Interrupt
		if (TIM_INT_Status == TIM_INT_ENABLE)
		{
			SET_BIT(TIMx->DIER, DIER_CC1IE);
		}
		else if (TIM_INT_Status == TIM_INT_DISABLE)
		{
			CLR_BIT(TIMx->DIER, DIER_CC1IE);
		}

		SET_BIT(TIMx->CCER, CCER_CC4E);  // Enable capture/compare channel
		break;
	}
	TIMx->ARR = TIM_MAX_PERIOD;
	SET_BIT(TIMx->CR1, CR1_CEN); // Enable timer/counter
}

/**
 * @brief  Enables the interrupt for the specified timer's capture/compare events.
 * @param  TIMx : Pointer to the timer peripheral (TIM1, TIM2, or TIM3).
 * @retval None
 */
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

/**
 * @brief  Sets the callback function for the TIM1 update event interrupt
 * @param  functionPtr :  Pointer to the callback function.
 * @retval None
 */
void TIM1_UP_SetCallback(void (*functionPtr)(void))
{
	TIM1_UP_Callback_Ptr = functionPtr;
}

/**
 * @brief  Sets the callback function for the TIM1 trigger and communication interrupts.
 * @param  functionPtr :  Pointer to the callback function.
 * @retval None
 */
void TIM1_TRG_COM_SetCallback(void (*functionPtr)(void))
{
	TIM1_TRG_COM_Callback_Ptr = functionPtr;
}

/**
 * @brief  Sets the callback function for the capture/compare interrupt
 * @param  functionPtr :  Pointer to the callback function.
 * @retval None
 */
void TIM1_CC_SetCallback(void (*functionPtr)(void))
{
	TIM1_CC_Callback_Ptr = functionPtr;
}
/**
 * @brief  Sets the callback function for the TIM2 interrupt.
 * @param  functionPtr :  Pointer to the callback function.
 * @retval None
 */
void TIM2_SetCallback(void (*functionPtr)(void))
{
	TIM2_Callback_Ptr = functionPtr;
}
/**
 * @brief  Sets the callback function for the TIM3 interrupt.
 * @param  functionPtr :  Pointer to the callback function.
 * @retval None
 */
void TIM3_SetCallback(void (*functionPtr)(void))
{
	TIM3_Callback_Ptr = functionPtr;
}

/**
 * @brief  TIM1 update event interrupt handler.
 * @param  None
 * @retval None
 */
void TIM1_UP_IRQHandler(void)
{
	if (TIM1_UP_Callback_Ptr != 0)
	{
		TIM1_UP_Callback_Ptr();
	}
}

/**
 * @brief  TIM1 trigger and communication interrupt handler.
 * @param  None
 * @retval None
 */
void TIM1_TRG_COM_IRQHandler(void)
{
	if (TIM1_TRG_COM_Callback_Ptr != 0)
	{
		TIM1_TRG_COM_Callback_Ptr();
	}
}

/**
 * @brief  TIM1 capture/compare interrupt handler.
 * @param  None
 * @retval None
 */
void TIM1_CC_IRQHandler(void)
{
	if (TIM1_CC_Callback_Ptr != 0)
	{
		TIM1_CC_Callback_Ptr();
	}
}

/**
 * @brief  TIM2 interrupt handler.
 * @param  None
 * @retval None
 */
void TIM2_IRQHandler(void)
{
	if (TIM3_Callback_Ptr != 0)
	{
		TIM2_Callback_Ptr();
	}
}

/**
 * @brief  TIM3 interrupt handler.
 * @param  None
 * @retval None
 */
void TIM3_IRQHandler(void)
{

	if (TIM3_Callback_Ptr != 0)
	{
		TIM3_Callback_Ptr();
	}

}
