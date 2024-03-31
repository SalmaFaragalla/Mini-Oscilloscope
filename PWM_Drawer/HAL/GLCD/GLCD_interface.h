/*
 * GLCD_interface.h
 */

#ifndef GLCD_GLCD_INTERFACE_H_
#define GLCD_GLCD_INTERFACE_H_

#include "../Inc/STD_TYPES.h"

typedef enum
{
	GLCD_LINE_0 = 0x00,
	GLCD_LINE_1,
	GLCD_LINE_2,
	GLCD_LINE_3,
	GLCD_LINE_4,
	GLCD_LINE_5,
	GLCD_LINE_6,
	GLCD_LINE_7,
}GLCD_LineNum_t;


void GLCD_Init(void);

void GLCD_PrintChar(u8 data, u8 x, GLCD_LineNum_t y);
void GLCD_PrintString(string str, u8 x, GLCD_LineNum_t y);
void GLCD_PrintNum(u32 num , u8 x , GLCD_LineNum_t y);
void GLCD_PrintFloat(f32 num , u8 x ,GLCD_LineNum_t y);

void GLCD_ClearLine(GLCD_LineNum_t line);
void GLCD_DrawHLine(u8 x1, u8 x2, GLCD_LineNum_t y);
void GLCD_DrawVLine(GLCD_LineNum_t y1, GLCD_LineNum_t y2, u8 x);

#endif /* GLCD_GLCD_INTERFACE_H_ */
