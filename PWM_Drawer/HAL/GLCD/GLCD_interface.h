/**
 ******************************************************************************
 * @file    GLCD_interface.h
 * @author  Salma Faragalla
 * @ brief  Header file of GLCD module.
 ******************************************************************************
 */
#ifndef GLCD_GLCD_INTERFACE_H_
#define GLCD_GLCD_INTERFACE_H_

#include "../Inc/STD_TYPES.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @typedef GLCD_LineNum_t
 * @brief Enumeration of GLCD line number.
 */
typedef enum
{
	GLCD_LINE_0 = 0UL,
	GLCD_LINE_1,
	GLCD_LINE_2,
	GLCD_LINE_3,
	GLCD_LINE_4,
	GLCD_LINE_5,
	GLCD_LINE_6,
	GLCD_LINE_7,
}GLCD_LineNum_t;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initializes GLCD display
 * @param  None
 * @retval None
 */
void GLCD_Init(void);
/**
 * @brief  Prints a single character on  GLCD display
 * @param  x : X coordinate
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
void GLCD_PrintChar(u8 data, u8 x, GLCD_LineNum_t y);
/**
 * @brief  Prints a string on  GLCD display
 * @param  x : X coordinate
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
void GLCD_PrintString(string str, u8 x, GLCD_LineNum_t y);

/**
 * @brief  Prints an unsigned 32-bit integer as a string on the GLCD display.
 * @param  x : X coordinate
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
void GLCD_PrintNum(u32 num , u8 x , GLCD_LineNum_t y);

/**
 * @brief  Prints an float 32-bit integer as a string on the GLCD display.
 * @param  x : X coordinate
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
void GLCD_PrintFloat(f32 num , u8 x ,GLCD_LineNum_t y);
/**
 * @brief  Clears a specific line on the GLCD display
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
void GLCD_ClearLine(GLCD_LineNum_t line);
/**
 * @brief  Draws a horizontal line on GLCD display
 * @param  x1 : The starting X coordinate (column) of the line.
 * @param  x2 : The ending X coordinate (column) of the line.
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
void GLCD_DrawHLine(u8 x1, u8 x2, GLCD_LineNum_t y);

/**
 * @brief  Draws a horizontal line on GLCD display
 * @param  y1 : The starting line number of the line..
 * @param  y2 : The ending line number of the line.
 * @param  x : x coordinate
 * @retval None
 */
void GLCD_DrawVLine(GLCD_LineNum_t y1, GLCD_LineNum_t y2, u8 x);

#endif /* GLCD_GLCD_INTERFACE_H_ */
