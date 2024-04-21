/**
 ******************************************************************************
 * @file    NVIC_program.c
 * @author  Salma Faragalla
 * @ brief  NVIC module driver
 ******************************************************************************
 */
/* Includes -------------------------------------------------------------------*/
#include "STD_TYPES.h"

#include "NVIC_interface.h"

/* Public functions -------------------------------------------------------------*/
/**
 * @brief Enables the specified interrupt in the Nested Vectored Interrupt Controller.
 * @param IRQn : Interrupt number to be enabled.
 * @retval None
 */
void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  if ((u32)(IRQn) >= 0)
  {
    NVIC->ISER[(((u32)IRQn) >> 5UL)] = (u32)(1UL << (((u32)IRQn) & 0x1FUL));
  }
}
