/**
 ******************************************************************************
 * @file    GPIO_program.c
 * @author  Salma Faragalla
 * @ brief  GPIO module driver
 ******************************************************************************
 */
/* Includes ----------------------------------------------------------------*/
#include "../../Inc/BIT_MATH.h"

#include "../GPIO/GPIO_interface.h"

/* Public Functions --------------------------------------------------------*/
/**
 * @brief  Sets the direction and speed for a specific pin on a GPIO port.
 * @param GPIOx : Pointer to the GPIO port (GPIOA, GPIOB, GPIOC ).
 * @param GPIO_PINx : Pin number within the port (e.g., GPIO_PIN0, GPIO_PIN1, etc.).
 * @param GPIO_Mode : Mode for the pin (direction and speed).
 * @retval None
 */
void GPIO_SetPinDirSpeed(volatile GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx, GPIO_Mode_t GPIO_Mode)
{
	/* Check if the current bit belongs to first half or last half of the pin count number
	   in order to address CRH or CRL register*/
	if ((GPIO_PINx < GPIO_PIN8))
	{
		GPIOx->CRL &= ~((0b1111) << (GPIO_PINx * GPIO_PIN_MODE_OFFSET)); // Clear mode bits
		GPIOx->CRL |= (GPIO_Mode << (GPIO_PINx * GPIO_PIN_MODE_OFFSET)); // Set mode bits
	}
	else
	{
		GPIOx->CRH &= ~((0b1111) << ((GPIO_PINx - 8) * GPIO_PIN_MODE_OFFSET)); // Clear mode bits
		GPIOx->CRH |= (GPIO_Mode << ((GPIO_PINx - 8) * GPIO_PIN_MODE_OFFSET)); // Set mode bits
	}

}

/**
 * @brief  Sets the direction and speed for a specific pin on a GPIO port.
 * @param GPIOx : Pointer to the GPIO port (GPIOA, GPIOB, GPIOC ).
 * @param GPIO_PINx : Pin number within the port (e.g., GPIO_PIN0, GPIO_PIN1, etc.).
 * @param GPIO_PIN_Val : Value to set on the pin (GPIO_PIN_HIGH or GPIO_PIN_LOW).
 * @retval None
 */
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

/**
 * @brief  Gets the current value (high or low) for a specific pin on a GPIO port.
 * @param GPIOx : Pointer to the GPIO port (GPIOA, GPIOB, GPIOC ).
 * @param GPIO_PINx : Pin number within the port (e.g., GPIO_PIN0, GPIO_PIN1, etc.).
 * @retval None
 */
GPIO_PinVal_t GPIO_GetPinValue(volatile GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx)
{
	return GET_BIT(GPIOx->IDR,GPIO_PINx);
}

/**
 * @brief  Enables the internal pull-up resistor for a specific pin on a GPIO port.
 * @param GPIOx : Pointer to the GPIO port (GPIOA, GPIOB, GPIOC ).
 * @param GPIO_PINx : Pin number within the port (e.g., GPIO_PIN0, GPIO_PIN1, etc.).
 * @retval None
 */
void GPIO_SetPullup(volatile GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx)
{
	SET_BIT(GPIOx->ODR, GPIO_PINx);
}

/**
 * @brief  Sets the direction and speed for all pins on a GPIO port.
 * @param GPIOx : Pointer to the GPIO port (GPIOA, GPIOB, GPIOC ).
 * @param GPIO_Mode : Mode for the pin (direction and speed).
 * @retval None
 */
void GPIO_SetPortDirSpeed (volatile  GPIO_TypeDef *GPIOx ,GPIO_Mode_t GPIO_Mode )
{
	u32 portMode=0;
	for(int i=0 ; i<GPIO_PIN8 ; ++i)
	{
		// Combine the shifted mode with the existing port mode for all pins
		portMode |= ((GPIO_Mode) << (i *GPIO_PIN_MODE_OFFSET));
	}

	GPIOx->CRL = portMode;
	GPIOx->CRH = portMode;
}
/**
 * @brief  Sets the value a GPIO port.
 * @param  GPIOx : Pointer to the GPIO port (GPIOA, GPIOB, GPIOC ).
 * @param  val : Value to set on the GPIO port.
 * @retval None
 */
void GPIO_SetPortValue( volatile  GPIO_TypeDef *GPIOx , u32 val)
{
	GPIOx->ODR = (u32)val;
}
