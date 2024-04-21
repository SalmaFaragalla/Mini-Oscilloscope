/**
 ******************************************************************************
 * @file    RCC_private.h
 * @author  Salma Faragalla
 ******************************************************************************
 */

#ifndef RCC_RCC_PRIVATE_H_
#define RCC_RCC_PRIVATE_H_

typedef struct
{
	volatile u32 CR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 APB2RSTR;
	volatile u32 APB1RSTR;
	volatile u32 AHBENR;
	volatile u32 APB2ENR;
	volatile u32 APB1ENR;
	volatile u32 BDCR;
	volatile u32 CSR;
	volatile u32 AHBSTR;
	volatile u32 CFGR2;

} RCC_TypeDef;

#define RCC ((volatile RCC_TypeDef*)0x40021000UL)

#define RCC_GPIOA_CLK_EN()	( RCC->APB2ENR |= (0x01UL<<2) )
#define RCC_GPIOB_CLK_EN()	( RCC->APB2ENR |= (0x01UL<<3) )
#define RCC_GPIOC_CLK_EN()	( RCC->APB2ENR |= (0x01UL<<4) )

#define RCC_AFIO_CLK_EN()   (RCC ->APB2ENR |= (0x01UL<<0) )

#define RCC_TIM1_CLK_EN()   (RCC->APB2ENR  |= (0x01UL<<11))
#define RCC_TIM2_CLK_EN()   (RCC->APB1ENR  |= (0x01UL<<0) )
#define RCC_TIM3_CLK_EN()   (RCC->APB1ENR  |= (0x01UL<<1) )


#endif /* RCC_RCC_PRIVATE_H_ */
