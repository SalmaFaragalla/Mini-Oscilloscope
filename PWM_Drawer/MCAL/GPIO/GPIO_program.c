/*
 * GPIO_program.c
 */

/* Includes ----------------------------------------------------------------*/
#include "../../Inc/BIT_MATH.h"

#include "../GPIO/GPIO_interface.h"

/* Public Functions --------------------------------------------------------*/
void GPIO_SetPinDirSpeed(volatile GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx, GPIO_Mode_t GPIO_Mode)
{
	/* Check if the current bit belongs to first half or last half of the pin count number
	   in order to address CRH or CRL register*/
	if ((GPIO_PINx < GPIO_PIN8))
	{
		GPIOx->CRL &= ~((0b1111) << (GPIO_PINx * GPIO_PIN_MODE_OFFSET));
		GPIOx->CRL |= (GPIO_Mode << (GPIO_PINx * GPIO_PIN_MODE_OFFSET));
	}
	else
	{
		GPIOx->CRH &= ~((0b1111) << ((GPIO_PINx - 8) * GPIO_PIN_MODE_OFFSET));
		GPIOx->CRH |= (GPIO_Mode << ((GPIO_PINx - 8) * GPIO_PIN_MODE_OFFSET));
	}

}
void GPIO_SetPinValue(volatile  GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx, GPIO_PinVal_t GPIO_PIN_Val)
{
	if (GPIO_PIN_Val == GPIO_PIN_HIGH)
	{
		SET_BIT(GPIOx->ODR, GPIO_PINx);
	}
	else if (GPIO_PIN_Val == GPIO_PIN_LOW)
	{
		CLR_BIT(GPIOx->ODR, GPIO_PINx);
	}

}
GPIO_PinVal_t GPIO_GetPinValue(volatile GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx)
{
	return GET_BIT(GPIOx->IDR,GPIO_PINx);
}
void GPIO_SetPullup(volatile GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx)
{
	SET_BIT(GPIOx->ODR, GPIO_PINx);
}


void GPIO_SetPortDirSpeed (volatile  GPIO_TypeDef *GPIOx ,GPIO_Mode_t GPIO_Mode )
{
	u32 temp=0;
	for(int i=0 ; i<GPIO_PIN8 ; ++i)
	{
		temp |= ((GPIO_Mode) << (i *GPIO_PIN_MODE_OFFSET));
	}

	GPIOx->CRL = temp;
	GPIOx->CRH = temp;
}

void GPIO_SetPortValue( volatile  GPIO_TypeDef *GPIOx , u32 val)
{
	GPIOx->ODR = (u32)val;
}
