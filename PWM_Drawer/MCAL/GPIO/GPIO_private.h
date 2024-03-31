/*
 * GPIO_private.h
 *
 *  Created on: Mar 29, 2024
 *      Author: Salma
 */

#ifndef GPIO_GPIO_PRIVATE_H_
#define GPIO_GPIO_PRIVATE_H_

#include "STD_TYPES.h"

typedef struct
{
	volatile u32 CRL;
	volatile u32 CRH;
	volatile u32 IDR;
	volatile u32 ODR;
	volatile u32 BSRR;
	volatile u32 BRR;
	volatile u32 LCKR;
} GPIO_TypeDef;

#define GPIOA		((volatile GPIO_TypeDef*)0x40010800UL)
#define GPIOB		((volatile GPIO_TypeDef*)0x40010C00UL)
#define GPIOC		((volatile GPIO_TypeDef*)0x40011000UL)


#define GPIO_PIN_MODE_OFFSET (0x04UL)
#define GPIO_BSRR_OFFSET (16UL)

#endif /* GPIO_GPIO_PRIVATE_H_ */
