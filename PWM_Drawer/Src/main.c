/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Salma Faragalla
 * @brief          : Main program body
 ******************************************************************************
 */

#include "../APP/APP_interface.h"

int main()
{
	APP_Init();
	APP_IC_Start();
	APP_PWM_Start(79, 600);
	APP_GLCD_Print_Init();

	while (1)
	{
		APP_GLCD_Update();

	}

}
