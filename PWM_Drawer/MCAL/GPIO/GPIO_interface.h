/*
 * GPIO_interface.h
 */

#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

#include "STD_TYPES.h"
#include "STM32F103.h"

typedef enum
{
  GPIO_PIN0 = 0,
  GPIO_PIN1,
  GPIO_PIN2,
  GPIO_PIN3,
  GPIO_PIN4,
  GPIO_PIN5,
  GPIO_PIN6,
  GPIO_PIN7,
  GPIO_PIN8,
  GPIO_PIN9,
  GPIO_PIN10,
  GPIO_PIN11,
  GPIO_PIN12,
  GPIO_PIN13,
  GPIO_PIN14,
  GPIO_PIN15,
} GPIO_PinNum_t;

typedef enum
{
  GPIO_INPUT_ANLOG = 0b0000U,
  GPIO_INPUT_FLOATING = 0b0100U,
  GPIO_INPUT_PU_PD = 0b1000U,

  // For Speed 1
  GPIO_OUTPUT_PP_10MHZ = 0b0001U,
  GPIO_OUTPUT_OD_10MHZ = 0b0101U,
  GPIO_OUTPUT_AF_PP_10MHZ = 0b1001U,
  GPIO_OUTPUT_AF_OD_10MHZ = 0b1101U,

  // For Speed 2
  GPIO_OUTPUT_PP_2MHZ = 0b0010U,
  GPIO_OUTPUT_OD_2MHZ = 0b0110U,
  GPIO_OUTPUT_AF_PP_2MHZ = 0b1010U,
  GPIO_OUTPUT_AF_OD_2MHZ = 0b1110U,

  // For Speed 3
  GPIO_OUTPUT_PP_50MHZ = 0b0011U,
  GPIO_OUTPUT_OD_50MHZ = 0b0111U,
  GPIO_OUTPUT_AF_PP_50MHZ = 0b1011U,
  GPIO_OUTPUT_AF_OD_50MHZ = 0b1111U,

} GPIO_Mode_t;

typedef enum
{
  GPIO_PIN_LOW = 0x00,
  GPIO_PIN_HIGH = 0x01,

} GPIO_PinVal_t;

#define  GPIO_PORT_LOW  (0x00000000U)
#define  GPIO_PORT_HIGH (0xFFFFFFFFU)

void GPIO_SetPinDirSpeed(volatile GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx, GPIO_Mode_t GPIO_Mode);
void GPIO_SetPinValue( volatile  GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx, GPIO_PinVal_t GPIO_PIN_Val);
GPIO_PinVal_t GPIO_GetPinValue(volatile  GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx);
void GPIO_SetPullup(volatile GPIO_TypeDef *GPIOx, GPIO_PinNum_t GPIO_PINx);

void GPIO_SetPortDirSpeed (volatile  GPIO_TypeDef *GPIOx ,GPIO_Mode_t GPIO_Mode );
void GPIO_SetPortValue( volatile  GPIO_TypeDef *GPIOx , u32 val);

#endif /* GPIO_INTERFACE_H_ */
