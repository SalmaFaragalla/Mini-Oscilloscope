/**
 ******************************************************************************
 * @file    NVIC_interface.h
 * @author  Salma Faragalla
 * @ brief  Header file of NVIC module.
 ******************************************************************************
 */
#ifndef NVIC_NVIC_INTERFACE_H_
#define NVIC_NVIC_INTERFACE_H_

#include "STM32F103.h"

/**
 * @brief Enables the specified interrupt in the Nested Vectored Interrupt Controller.
 * @param IRQn : Interrupt number to be enabled.
 * @retval None
 */
void NVIC_EnableIRQ(IRQn_Type IRQn);

#endif /* NVIC_NVIC_INTERFACE_H_ */
