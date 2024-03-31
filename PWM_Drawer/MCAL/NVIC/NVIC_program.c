/*
 * NVIC_program.c
 */

/* Includes -------------------------------------------------------------------*/
#include "STD_TYPES.h"

#include "NVIC_interface.h"

/* Public functions -------------------------------------------------------------*/
void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  if ((u32)(IRQn) >= 0)
  {
    NVIC->ISER[(((u32)IRQn) >> 5UL)] = (u32)(1UL << (((u32)IRQn) & 0x1FUL));
  }
}
