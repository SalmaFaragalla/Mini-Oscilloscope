/**
 ******************************************************************************
 * @file    APP_program.c
 * @author  Salma Faragalla
 * @brief   Application program for PWM Drawer Application
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "BIT_MATH.h"

#include "../MCAL/TIM/TIM_interface.h"
#include "../HAL/GLCD/GLCD_interface.h"
#include "../MCAl/GPIO/GPIO_interface.h"

#include "APP_interface.h"
#include "APP_config.h"

/* Private functions prototypes ----------------------------------------------*/
/**
 * @brief  Calculates frequency and duty cycle based on captured timer values.
 * @param  None
 * @retval None
 */
static void APP_IC_Calculate_Freq_Duty(void);
/**
 * @brief  Draws a PWM signal on the GLCD.
 * @param  duty: Duty cycle value (0-100)
 * @retval None
 */
static void APP_GLCD_DrawPWM(u32 duty);
/**
 * @brief  Prints the current frequency value on the GLCD.
 * @param  None
 * @retval None
 */
static void APP_GLCD_PrintFreq();
/**
 * @brief  Prints the current duty cycle value on the GLCD.
 * @param  None
 * @retval None
 */
static void APP_GLCD_PrintDuty();
/**
 * @brief  Prints the current period value on the GLCD.
 * @param  None
 * @retval None
 */
static void APP_GLCD_PrintPeriod();
/**
 * @brief  Returns the current frequency value in KHZ.
 * @param  None
 * @retval Frequency in KHZ
 */
static f32 APP_IC_GetFreq_KHZ();
/**
 * @brief  Returns the current duty value .
 * @param  None
 * @retval Period in ms
 */
static f32 APP_IC_GetPeriod_ms();
/**
 * @brief  Returns the current duty value .
 * @param  None
 * @retval Duty (0-100)
 */
static u32 APP_IC_GetDuty();

/* Private variables --------------------------------------------------------*/
static f32 frequency = 0;
static u32 duty = 0;

static f32 oldFreq = 0;
static f32 oldDuty = 0;

/* Private functions --------------------------------------------------------*/

/**
 * @brief  Calculates frequency and duty cycle based on captured timer values.
 * @param  None
 * @retval None
 */
static void APP_IC_Calculate_Freq_Duty(void)
{
	// Flag to track if it's the first time a rising edge is captured
	static u8 IsFirstCapture = 0;

	// Variables to store captured time stamps (CCR register values)
	static u32 firstRisingEdgeCount = 0;
	static u32 secondRisingEdgeCount = 0;
	static u32 fallingEdgeCount = 0;

	// Variables to calculate differences between time stamps
	static u32 risingFallingEdgeDifference = 0;
	static u32 risingEdgeDifference = 0;

	// Variables to hold captured CCR register values for frequency and duty cycle calculation
	u32 TIMx_Freq_CCRx = 0;
	u32 TIMx_Duty_CCRx = 0;

	// Determine the capture channel for frequency measurement
	switch ( APP_TIM_IC_CH1)
	{
	case TIM_CH1:
		TIMx_Freq_CCRx =  APP_TIM_IC_TIMx->CCR1;
		break;

	case TIM_CH2:
		TIMx_Freq_CCRx =  APP_TIM_IC_TIMx->CCR2;
		break;

	case TIM_CH3:
		TIMx_Freq_CCRx =  APP_TIM_IC_TIMx->CCR3;
		break;

	case TIM_CH4:
		TIMx_Freq_CCRx =  APP_TIM_IC_TIMx->CCR4;
		break;
	}

	// Determine the capture channel for duty cycle measurement
	switch ( APP_TIM_IC_CH2)
	{
	case TIM_CH1:
		TIMx_Duty_CCRx =  APP_TIM_IC_TIMx->CCR1;
		break;

	case TIM_CH2:
		TIMx_Duty_CCRx =  APP_TIM_IC_TIMx->CCR2;
		break;

	case TIM_CH3:
		TIMx_Duty_CCRx =  APP_TIM_IC_TIMx->CCR3;
		break;

	case TIM_CH4:
		TIMx_Duty_CCRx =  APP_TIM_IC_TIMx->CCR4;
		break;
	}

	// Check if it's the first capture
	if (IsFirstCapture == 0)
	{
		firstRisingEdgeCount = TIMx_Freq_CCRx;
		IsFirstCapture = 1;
	}
	else
	{
		// Second rising edge captured implies falling edge was also captured
		secondRisingEdgeCount = TIMx_Freq_CCRx;
		fallingEdgeCount = TIMx_Duty_CCRx;

		//Calculate the difference between rising edges (handling counter overflow)
		if (secondRisingEdgeCount > firstRisingEdgeCount)
		{
			risingEdgeDifference = secondRisingEdgeCount - firstRisingEdgeCount;
		}
		else
		{
			risingEdgeDifference = (TIM_MAX_PERIOD - firstRisingEdgeCount) + secondRisingEdgeCount;
		}

		// Calculate the difference between rising and falling edges (handling counter overflow)
		if (fallingEdgeCount > firstRisingEdgeCount)
		{
			risingFallingEdgeDifference = fallingEdgeCount - firstRisingEdgeCount;
		}
		else
		{
			risingFallingEdgeDifference = (TIM_MAX_PERIOD - firstRisingEdgeCount) + fallingEdgeCount;
		}

		// Calculating the frequency and duty only if rising edge difference is significant
		if (risingEdgeDifference > 1)
		{
			frequency = (float)TIM_CLK / (float)risingEdgeDifference;
			duty = (risingFallingEdgeDifference * 100 ) / (float)risingEdgeDifference;
			IsFirstCapture = 0;
		}
	}
}

/**
 * @brief  Draws a PWM signal on the GLCD.
 * @param  duty: Duty cycle value (0-100)
 * @retval None
 */
static void APP_GLCD_DrawPWM(u32 duty)
{
	// Validate the duty cycle parameter
	if (duty < 0 || duty > 100)
	{
		// Duty cycle out of range, do not proceed with drawing
	    return;
	}

	//Clear the lines where the PWM will be drawn
	for (int i=APP_GLCD_HIGH_LINE ; i<=APP_GLCD_LOW_LINE ; ++i)
	{
		GLCD_ClearLine(i);
	}

	// Calculate the pixel range of the duty cycle
	int dutyXrange = (duty * 60) / 100;


	// Draw the baseline for the entire graph
	GLCD_DrawHLine(0, APP_GLCD_CYCLE_1_START_X, APP_GLCD_LOW_LINE);

	// Drawing the first cycle
	// Vertical line representing rising edge
	GLCD_DrawVLine(APP_GLCD_HIGH_LINE, APP_GLCD_LOW_LINE, APP_GLCD_CYCLE_1_START_X);
	// Top horizontal line representing duty cycle (on)
	GLCD_DrawHLine(APP_GLCD_CYCLE_1_START_X + 1, (APP_GLCD_CYCLE_1_START_X + dutyXrange), APP_GLCD_HIGH_LINE);
	// Vertical line representing falling edge
	GLCD_DrawVLine(APP_GLCD_HIGH_LINE, APP_GLCD_LOW_LINE, APP_GLCD_CYCLE_1_START_X + dutyXrange);
	// Lower horizontal line representing duty cycle (off)
	GLCD_DrawHLine(APP_GLCD_CYCLE_1_START_X + dutyXrange, APP_GLCD_CYCLE_2_START_X, APP_GLCD_LOW_LINE);

	// Drawing the second cycle
	// Vertical line representing rising edge
	GLCD_DrawVLine(APP_GLCD_HIGH_LINE, APP_GLCD_LOW_LINE, APP_GLCD_CYCLE_2_START_X);
	// Top horizontal line representing duty cycle (on)
	GLCD_DrawHLine(APP_GLCD_CYCLE_2_START_X + 1, (APP_GLCD_CYCLE_2_START_X + dutyXrange), APP_GLCD_HIGH_LINE);
	// Vertical line representing falling edge
	GLCD_DrawVLine(APP_GLCD_HIGH_LINE, APP_GLCD_LOW_LINE, APP_GLCD_CYCLE_2_START_X + dutyXrange);
	// Lower horizontal line representing duty cycle (off)
	GLCD_DrawHLine(APP_GLCD_CYCLE_2_START_X + dutyXrange, APP_GLCD_CYCLE_2_END_X, APP_GLCD_LOW_LINE);
}

/**
 * @brief  Prints the current frequency value on the GLCD.
 * @param  None
 * @retval None
 */
static void APP_GLCD_PrintFreq()
{
	GLCD_ClearLine(APP_GLCD_FREQ_LINE);
	f32 glcdFreq = APP_IC_GetFreq_KHZ();
	GLCD_PrintString("FREQ:", 0, APP_GLCD_FREQ_LINE);
	GLCD_PrintFloat(glcdFreq, 35, APP_GLCD_FREQ_LINE);
	GLCD_PrintString("KHZ", 90, APP_GLCD_FREQ_LINE);
}
/**
 * @brief  Prints the current duty cycle value on the GLCD.
 * @param  None
 * @retval None
 */
static void APP_GLCD_PrintDuty()
{
	GLCD_ClearLine(APP_GLCD_DUTY_LINE);
	u32 glcdDuty = APP_IC_GetDuty();
	GLCD_PrintString("DUTY:", 0, APP_GLCD_DUTY_LINE);
	GLCD_PrintNum(glcdDuty, 35, APP_GLCD_DUTY_LINE);
	GLCD_PrintString("%", 55, APP_GLCD_DUTY_LINE);
}
/**
 * @brief  Prints the current period value on the GLCD.
 * @param  None
 * @retval None
 */
static void APP_GLCD_PrintPeriod()
{
	GLCD_ClearLine(APP_GLCD_PERIOD_LINE);
	f32 glcdPeriod = APP_IC_GetPeriod_ms();
	GLCD_PrintString("PERIOD:", 0, APP_GLCD_PERIOD_LINE);
	GLCD_PrintFloat(glcdPeriod, 50, APP_GLCD_PERIOD_LINE);
	GLCD_PrintString("ms", 100, APP_GLCD_PERIOD_LINE);
}

/**
 * @brief  Returns the current frequency value in KHZ.
 * @param  None
 * @retval Frequency in KHZ
 */
static f32 APP_IC_GetFreq_KHZ()
{
	return frequency / 1000;
}

/**
 * @brief  Returns the current period value in ms.
 * @param  None
 * @retval period in ms
 */
static f32 APP_IC_GetPeriod_ms()
{
	int currFreq = frequency;

	// Ensure non-zero frequency to avoid division by zero
	if (frequency == 0)
		currFreq = 1;

	return ((1.0 / (currFreq)) * 1000);
}

/**
 * @brief  Returns the current duty value .
 * @param  None
 * @retval Duty (0-100)
 */
static u32 APP_IC_GetDuty()
{
	return duty;
}

/* Public functions --------------------------------------------------------*/

/**
 * @brief  Initializes the GLCD and timers used for PWM and input capture.
 * @param  None
 * @retval None
 */
void APP_Init(void)
{
	GLCD_Init();
	TIM_Init(APP_TIM_PWM_TIMx);
	TIM_Init( APP_TIM_IC_TIMx);
}

/**
 * @brief Starts the input capture timer for frequency and duty cycle calculations.
 * @param  None
 * @retval None
 */
void APP_IC_Start()
{
	TIM_IC_Start( APP_TIM_IC_TIMx,  APP_TIM_IC_CH1, CCS_IP_DIRECT, TIM_IC_RISING_EDGE , TIM_INT_ENABLE);

	TIM_IC_INT_Enable( APP_TIM_IC_TIMx);

	if ( APP_TIM_IC_TIMx == TIM1)
	{
		TIM1_CC_SetCallback(APP_IC_Calculate_Freq_Duty);
	}

	else if ( APP_TIM_IC_TIMx == TIM2)
	{
		TIM2_SetCallback(APP_IC_Calculate_Freq_Duty);
	}
	else if ( APP_TIM_IC_TIMx == TIM3)
	{
		TIM3_SetCallback(APP_IC_Calculate_Freq_Duty);
	}

	TIM_IC_Start(APP_TIM_IC_TIMx,  APP_TIM_IC_CH2, CCS_IP_DIRECT, TIM_IC_FALLING_EDGE ,  TIM_INT_DISABLE);

}

/**
 * @brief Starts the timer in PWM output mode.
 * @param  None
 * @retval None
 */
void APP_PWM_Start(u32 dutyCycle, u32 frequency)
{

	TIM_PWM_Start(APP_TIM_PWM_TIMx, APP_TIM_PWM_CHx, dutyCycle, frequency);
}

/**
 * @brief  Prints on GLCD the initial frequency, duty, period, and draws initial PWM signal
 * @param  None
 * @retval None
 */
void APP_GLCD_Print_Init()
{
	 // Retrieve initial frequency, duty cycle, and period values
	oldFreq = APP_IC_GetFreq_KHZ();
	oldDuty = APP_IC_GetDuty();

	// Print initial frequency, duty cycle, and period values on the GLCD
	APP_GLCD_PrintFreq();
	APP_GLCD_PrintDuty();
	APP_GLCD_PrintPeriod();

	// Draw initial PWM signal on the GLCD
	APP_GLCD_DrawPWM(duty);

}
/**
 * @brief  Updates GLCD display with new frequency, duty, period, and PWM signal if they have changed.
 * @param  None
 * @retval None
 */
void APP_GLCD_Update()
{
	// Check for changes in frequency
	if (APP_IC_GetFreq_KHZ() != oldFreq)
	{
		// Update and print new frequency on the GLCD
		oldFreq = APP_IC_GetFreq_KHZ();
		APP_GLCD_PrintFreq();

		// Print new period on GLCD
		APP_GLCD_PrintPeriod();
	}

	// Check for changes in duty cycle
	if (APP_IC_GetDuty() != oldDuty)
	{
		// Update and print new duty on the GLCD
		oldDuty = APP_IC_GetDuty();
		APP_GLCD_PrintDuty();

		// Draw new PWM signal on GLCD
		APP_GLCD_DrawPWM(oldDuty);
	}


}
