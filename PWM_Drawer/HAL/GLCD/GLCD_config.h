/**
 ******************************************************************************
 * @file    GLCD_config.h
 * @author  Salma Faragalla
 * @brief   Configuration file for GLCD module.
 ******************************************************************************
 */
#ifndef GLCD_GLCD_CONFIG_H_
#define GLCD_GLCD_CONFIG_H_

#include "../../MCAL/GPIO/GPIO_interface.h"


/* GLCD ports & pins ----------------------------------------------*/
#define GLCD_DATA_PORT (GPIOA)
#define GLCD_D0_PIN (GPIO_PIN5)
#define GLCD_D1_PIN (GPIO_PIN4)
#define GLCD_D2_PIN (GPIO_PIN10)
#define GLCD_D3_PIN (GPIO_PIN11)
#define GLCD_D4_PIN (GPIO_PIN12)
#define GLCD_D5_PIN (GPIO_PIN13)
#define GLCD_D6_PIN (GPIO_PIN14)
#define GLCD_D7_PIN (GPIO_PIN15)

#define GLCD_CTRL_PORT (GPIOB)
#define GLCD_DI_PIN (GPIO_PIN9)
#define GLCD_RW_PIN (GPIO_PIN5)
#define GLCD_EN_PIN (GPIO_PIN8)
#define GLCD_CS1_PIN (GPIO_PIN7)
#define GLCD_CS2_PIN (GPIO_PIN6)

/* GLCD Screen Size ----------------------------------------------*/
#define GLCD_SCREEN_HALF_WIDTH (64U)
#define GLCD_SCREEN_WIDTH (128U)

/* Font Configurations -------------------------------------------*/
#define GLCD_FONT_CHAR_WIDTH (7U)
#define GLCD_FONT_ARRAY_COLS (8U)

/* GLCD Commands -------------------------------------------------*/
#define GLCD_CMD_OFF (0x3E)
#define GLCD_CMD_RESET_Y (0x40)
#define GLCD_CMD_RESET_X (0xB8)
#define GLCD_CMD_RESET_LINES (0xC0)
#define GLCD_CMD_ON (0x3F)

/* GLCD Delay ----------------------------------------------------*/
#define T 1

#endif /* GLCD_GLCD_CONFIG_H_ */
