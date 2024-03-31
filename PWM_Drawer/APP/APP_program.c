/*
 * APP_program.c
 */

/* Includes ------------------------------------------------------------------*/
#include "BIT_MATH.h"

#include "../MCAL/TIM/TIM_interface.h"
#include "../HAL/GLCD/GLCD_interface.h"
#include "../MCAl/GPIO/GPIO_interface.h"

#include "APP_interface.h"
#include "APP_config.h"

/* Private functions prototypes ----------------------------------------------*/
static void APP_IC_CalculateFreq(void);
static void APP_IC_CalculateDuty(void);
static void APP_GLCD_DrawPWM(u32 duty);
static f32 APP_IC_GetFreq_KHZ();
static f32 APP_IC_GetPeriod_ms();
static u32 APP_IC_GetDuty();
/* Private variables --------------------------------------------------------*/
static u8 IsFirstCapture = 0;

static u32 firstRisingEdgeCount = 0;
static u32 secondRisingEdgeCount = 0;
static u32 risingEdgeDifference = 0;

static u32 dutydiff = 0;

static f32 frequency = 0;
static u32 duty = 0;

/* Private functions --------------------------------------------------------*/
static void APP_IC_CalculateFreq(void)
{
	u32 TIMx_CCRx = 0;

	switch (APP_TIM_IC1_CHx)
	{
	case TIM_CH1:
		TIMx_CCRx = APP_TIM_IC1_TIMx->CCR1;
		break;

	case TIM_CH2:
		TIMx_CCRx = APP_TIM_IC1_TIMx->CCR2;
		break;

	case TIM_CH3:
		TIMx_CCRx = APP_TIM_IC1_TIMx->CCR3;
		break;

	case TIM_CH4:
		TIMx_CCRx = APP_TIM_IC1_TIMx->CCR4;
		break;
	}

	if (IsFirstCapture == 0)
	{
		firstRisingEdgeCount = TIMx_CCRx;
		IsFirstCapture = 1; // set the flag
	}
	else
	{
		secondRisingEdgeCount = TIMx_CCRx;
		if (secondRisingEdgeCount > firstRisingEdgeCount)
		{
			risingEdgeDifference = secondRisingEdgeCount - firstRisingEdgeCount;
		}
		else
		{
			risingEdgeDifference = (TIM_MAX_PERIOD - firstRisingEdgeCount) + secondRisingEdgeCount;
		}

		if (risingEdgeDifference > 1)
		{
			frequency = (float)TIM_CLK / (float)risingEdgeDifference;
			IsFirstCapture = 0; // reset the flag
		}
	}
}

static void APP_IC_CalculateDuty(void)
{

	u32 TIMx_CCRx = 0;

	switch (APP_TIM_IC2_CHx)
	{
	case TIM_CH1:
		TIMx_CCRx = APP_TIM_IC2_TIMx->CCR1;
		break;

	case TIM_CH2:
		TIMx_CCRx = APP_TIM_IC2_TIMx->CCR2;
		break;

	case TIM_CH3:
		TIMx_CCRx = APP_TIM_IC2_TIMx->CCR3;
		break;

	case TIM_CH4:
		TIMx_CCRx = APP_TIM_IC2_TIMx->CCR4;
		break;
	}

	if (risingEdgeDifference > 1)
	{
		if (TIMx_CCRx > firstRisingEdgeCount)
		{
			dutydiff = TIMx_CCRx - firstRisingEdgeCount;
		}
		else
		{
			dutydiff = (TIM_MAX_PERIOD - firstRisingEdgeCount) + TIMx_CCRx;
		}
		duty = (dutydiff * 100) / risingEdgeDifference;
	}
}

static void APP_GLCD_DrawPWM(u32 duty)
{
	int dutyXrange = (duty * 60) / 100;

	GLCD_DrawHLine(0, APP_GLCD_CYCLE_1_START_X, APP_GLCD_LOW_LINE);

	// Drawing the first cycle
	GLCD_DrawVLine(APP_GLCD_HIGH_LINE, APP_GLCD_LOW_LINE, APP_GLCD_CYCLE_1_START_X);
	GLCD_DrawHLine(APP_GLCD_CYCLE_1_START_X + 1, (APP_GLCD_CYCLE_1_START_X + dutyXrange), APP_GLCD_HIGH_LINE);
	GLCD_DrawVLine(APP_GLCD_HIGH_LINE, APP_GLCD_LOW_LINE, APP_GLCD_CYCLE_1_START_X + dutyXrange);
	GLCD_DrawHLine(APP_GLCD_CYCLE_1_START_X + dutyXrange, APP_GLCD_CYCLE_2_START_X, APP_GLCD_LOW_LINE);

	// Drawing the second cycle
	GLCD_DrawVLine(APP_GLCD_HIGH_LINE, APP_GLCD_LOW_LINE, APP_GLCD_CYCLE_2_START_X);
	GLCD_DrawHLine(APP_GLCD_CYCLE_2_START_X + 1, (APP_GLCD_CYCLE_2_START_X + dutyXrange), APP_GLCD_HIGH_LINE);
	GLCD_DrawVLine(APP_GLCD_HIGH_LINE, APP_GLCD_LOW_LINE, APP_GLCD_CYCLE_2_START_X + dutyXrange);
	GLCD_DrawHLine(APP_GLCD_CYCLE_2_START_X + dutyXrange, APP_GLCD_CYCLE_2_END_X, APP_GLCD_LOW_LINE);
}
/* Public functions --------------------------------------------------------*/
void APP_Init(void)
{
	GLCD_Init();
	TIM_Init(APP_TIM_IC1_TIMx);

	TIM_Init(APP_TIM_IC2_TIMx);

	TIM_Init(APP_TIM_PWM_TIMx);
}

void APP_IC_Start()
{
	TIM_IC_Start(APP_TIM_IC1_TIMx, APP_TIM_IC1_CHx, CCS_IP_DIRECT, TIM_IC_RISING_EDGE);
	TIM_IC_INT_Enable(APP_TIM_IC1_TIMx);

	if (APP_TIM_IC1_TIMx == TIM1)
	{
		TIM1_CC_SetCallback(APP_IC_CalculateFreq);
	}

	else if (APP_TIM_IC1_TIMx == TIM2)
	{
		TIM2_SetCallback(APP_IC_CalculateFreq);
	}
	else if (APP_TIM_IC1_TIMx == TIM3)
	{
		TIM3_SetCallback(APP_IC_CalculateFreq);
	}

	TIM_IC_Start(APP_TIM_IC2_TIMx, APP_TIM_IC2_CHx, CCS_IP_DIRECT, TIM_IC_FALLING_EDGE);
	TIM_IC_INT_Enable(APP_TIM_IC2_TIMx);

	if (APP_TIM_IC2_TIMx == TIM1)
	{
		TIM1_CC_SetCallback(APP_IC_CalculateDuty);
	}
	else if (APP_TIM_IC2_TIMx == TIM2)
	{
		TIM2_SetCallback(APP_IC_CalculateDuty);
	}

	if (APP_TIM_IC2_TIMx == TIM3)
	{
		TIM3_SetCallback(APP_IC_CalculateDuty);
	}
}

void APP_PWM_Start(u32 dutyCycle, u32 frequency)
{

	TIM_PWM_Start(APP_TIM_PWM_TIMx, APP_TIM_PWM_CHx, dutyCycle, frequency);
}

static f32 APP_IC_GetFreq_KHZ()
{
	// return frequency in KHZ
	return frequency / 1000;
}

f32 APP_IC_GetPeriod_ms()
{
	// return period in ms
	return ((1.0 / (frequency)) * 1000);
}

u32 APP_IC_GetDuty()
{
	return duty;
}

void APP_GLCD_Update()
{
	APP_GLCD_DrawPWM(duty);

	f32 numx = APP_IC_GetFreq_KHZ();
	GLCD_PrintString("FREQ:", 0, 0);
	GLCD_PrintFloat(numx, 35, 0);
	GLCD_PrintString("KHZ", 90, 0);

	u32 numy = APP_IC_GetDuty();
	GLCD_PrintString("DUTY:", 0, 1);
	GLCD_PrintNum(duty, 35, 1);
	GLCD_PrintString("%", 55, 1);

	f32 numz = APP_IC_GetPeriod_ms();
	GLCD_PrintString("PERIOD:", 0, 2);
	GLCD_PrintFloat(numz, 50, 2);
	GLCD_PrintString("ms", 100, 2);

	if (APP_IC_GetFreq_KHZ() != numx)
	{
		GLCD_ClearLine(0);
	}

	if (APP_IC_GetDuty() != numy)
	{
		GLCD_ClearLine(1);
	}
	if (APP_IC_GetPeriod_ms() != numz)
	{
		GLCD_ClearLine(2);
	}
}
