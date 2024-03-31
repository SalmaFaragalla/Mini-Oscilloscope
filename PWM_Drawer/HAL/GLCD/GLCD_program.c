/*
 * GLCD_program.c
 */

/* Includes ------------------------------------------------------------------*/
#include "BIT_MATH.h"
#include "STM32F103.h"

#include "../../MCAL/GPIO/GPIO_interface.h"

#include "GLCD_interface.h"
#include "GLCD_config.h"
#include "GLCD_font.h"

/* Private Function Prototypes-------------------------------------------------*/
static void Delay(u8 t);
static void GLCD_Send(u8 data);
static void GLCD_SendCommand(u8 command);
static void GLCD_SendData(u8 data, u8 x);
static void GLCD_GoToLine(GLCD_LineNum_t y);
static void GLCD_GoToColumn(u8 x);
static void GLCD_GoTo_Col_Line(u8 col, u8 line);
static void GLCD_reverseString(char* str, int length) ;


/* Private Functions ---------------------------------------------------------*/
static void Delay(u8 t)
{
	t *= 10;
	while (t)
	{
		t--;
	}
}
static void GLCD_Send(u8 data)
{
	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, GPIO_PIN_LOW);
	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_RW_PIN, GPIO_PIN_LOW);
	Delay(T);
	GPIO_SetPortValue(GLCD_DATA_PORT, GPIO_PORT_LOW);
	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, GPIO_PIN_HIGH);
	Delay(T);

	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D0_PIN,GET_BIT(data,0));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D1_PIN,GET_BIT(data,1));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D2_PIN,GET_BIT(data,2));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D3_PIN,GET_BIT(data,3));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D4_PIN,GET_BIT(data,4));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D5_PIN,GET_BIT(data,5));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D6_PIN,GET_BIT(data,6));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D7_PIN,GET_BIT(data,7));

	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, GPIO_PIN_LOW);
}

static void GLCD_SendCommand(u8 command)
{
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_DI_PIN, GPIO_PIN_LOW);
	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_HIGH);
	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_PIN_HIGH);
	Delay(T);
	GLCD_Send(command);
}

void GLCD_SendData(u8 data, u8 x)
{

	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_DI_PIN, GPIO_PIN_HIGH);
	Delay(T);

	if (x < GLCD_SCREEN_HALF_WIDTH)
	{
		// Left section
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_HIGH);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_PIN_LOW);
		Delay(T);
		GLCD_Send(data);
	}
	else
	{
		// Right section
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_LOW);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_PIN_HIGH);
		Delay(T);
		GLCD_Send(data);
	}
}

static void GLCD_GoToLine(GLCD_LineNum_t y)
{
	Delay(T);
	y = (y | 0xB8) & 0xBF;
	GLCD_SendCommand(y);
}

static void GLCD_GoToColumn(u8 x)
{
	Delay(T);
	if (x < GLCD_SCREEN_HALF_WIDTH) // Left section
	{
		x = (x | 0x40) & 0x7F;

		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_DI_PIN, GPIO_PIN_LOW);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_HIGH);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_PIN_LOW);
		Delay(T);
		GLCD_Send(x);
	}
	else // Right section
	{
		x -= GLCD_SCREEN_HALF_WIDTH;
		x = (x | 0x40) & 0x7F;

		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_DI_PIN, GPIO_PIN_LOW);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_LOW);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_PIN_HIGH);
		Delay(T);
		GLCD_Send(x);
	}
}

static void GLCD_GoTo_Col_Line(u8 col, GLCD_LineNum_t line)
{
	Delay(T);
	GLCD_GoToLine(line);	// Row Y = 0 to 7
	GLCD_GoToColumn(col); // Column x = 0 to 127

}

static void GLCD_reverseString(char* str, int length)
{
	    int start = 0;
	    int end = length - 1;
	    while (start < end)
	    {
	        char temp = str[start];
	        str[start] = str[end];
	        str[end] = temp;
	        start++;
	        end--;
	    }
}

/*Public Functions -------------------------------------------------*/
void GLCD_Init(void)
{
	/* Enable GPIO Clock*/
	RCC_GPIOA_CLK_EN();
	RCC_GPIOB_CLK_EN();

	/* Set GLCD ports direction */
	GPIO_SetPinDirSpeed(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_CTRL_PORT, GLCD_EN_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_CTRL_PORT, GLCD_DI_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_CTRL_PORT, GLCD_RW_PIN, GPIO_OUTPUT_PP_2MHZ);

	GPIO_SetPinDirSpeed(GLCD_DATA_PORT, GLCD_D0_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_DATA_PORT, GLCD_D1_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_DATA_PORT, GLCD_D2_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_DATA_PORT, GLCD_D3_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_DATA_PORT, GLCD_D4_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_DATA_PORT, GLCD_D5_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_DATA_PORT, GLCD_D6_PIN, GPIO_OUTPUT_PP_2MHZ);
	GPIO_SetPinDirSpeed(GLCD_DATA_PORT, GLCD_D7_PIN, GPIO_OUTPUT_PP_2MHZ);

	/* Send commands to initialize GLCD */
	Delay(T);
		GLCD_SendCommand(GLCD_CMD_OFF);
		GLCD_SendCommand(GLCD_CMD_RESET_Y);
		GLCD_SendCommand(GLCD_CMD_RESET_X);
		GLCD_SendCommand(GLCD_CMD_RESET_LINES);
		GLCD_SendCommand(GLCD_CMD_ON);

}


void GLCD_PrintChar(u8 data, u8 x, u8 y)
{
	int i,j;
	i=0;
	j = i + GLCD_FONT_CHAR_WIDTH;
	if (x >= (GLCD_SCREEN_WIDTH - GLCD_FONT_CHAR_WIDTH))
					{
							++y;   // Next line
							x=0; // Beginning line
					}
	for (i=0; i <= j; i++)
			{
				GLCD_GoTo_Col_Line(x, y);
				GLCD_SendData(Font[data][i], x); //
				++x;					  // Next position
			}

}

void GLCD_PrintString(string str, u8 x, GLCD_LineNum_t y)
{
	while (*str != 0)
	{
		if (x + GLCD_FONT_CHAR_WIDTH >=GLCD_SCREEN_WIDTH)
		{
			++y;
			x=0;
		}
		GLCD_PrintChar(*str, x, y);
		x+=GLCD_FONT_CHAR_WIDTH;
		str++;
	}
}

void GLCD_PrintNum(u32 num , u8 x , u8 y)
{
		u8 str [10];

		if (num == 0)
		{
			str[0]= '0';
			str[1]= '\0';
			GLCD_PrintString(str, x, y);
			return;
		}

	    int i, rem, len, n;

	    n = num;
	    len=0;

	    while (n != 0)
	    {
	        len++;
	        n /= 10;
	    }

	    for (i = 0; i < len; i++)
	    {
	        rem = num % 10;
	        num /= 10;
	        str[len - (i + 1)] = rem + '0';
	    }
	    str[len] = '\0';

	    GLCD_PrintString(str, x, y);
	    return;

}

void GLCD_PrintFloat(f32 num , u8 x , u8 y)
{
		char str[50];

	    int integerPart = (int)num;
	    float fractionalPart = num - integerPart;

	    int i = 0;

	    // Handle negative numbers
	    if (integerPart < 0) {
	        str[i++] = '-';
	        integerPart = -integerPart;
	        fractionalPart = -fractionalPart;
	    }

	    // Convert integer part to string
	    do {
	        str[i++] = '0' + integerPart % 10;
	        integerPart /= 10;
	    } while (integerPart);

	    GLCD_reverseString(str, i);

	    // Add decimal point
	    str[i++] = '.';

	    // Convert fractional part to string
	    for (int j = 0; j < 4; j++) {
	        fractionalPart *= 10;
	        int digit = (int)fractionalPart;
	        str[i++] = '0' + digit;
	        fractionalPart -= digit;
	    }

	    // Null-terminate the string
	    str[i] = '\0';

	    GLCD_PrintString(str, x, y);
}




void GLCD_ClearLine(GLCD_LineNum_t line)
{
	Delay(T);
	int i;
	GLCD_GoTo_Col_Line(0, line);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_HIGH);
	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_LOW);

	for (i = 0; i < 64; i++)
	{
		GLCD_SendData(0, 0);	 // Print 0 for Delete Left section
		GLCD_SendData(0, 64); // Print 0 for Delete Right section
	}
	GLCD_GoTo_Col_Line(64, line); // At start of line of right side
	for (i = 0; i < 64; i++)
	{
		GLCD_SendData(0, 0);	 // Print 0 for Delete Left section
		GLCD_SendData(0, 64); // Print 0 for Delete Right section
	}
}



void GLCD_DrawHLine(u8 x1, u8 x2, GLCD_LineNum_t y)
{
	for (char x=x1;x<=x2;x++)
	{
		GLCD_GoTo_Col_Line (x,y);
		GLCD_SendData(0x01, x);
	}

}

void GLCD_DrawVLine(GLCD_LineNum_t y1, GLCD_LineNum_t y2, u8 x)
{
	for (char y=y1;y<y2;y++)
	{
		GLCD_GoTo_Col_Line (x,y);
		GLCD_SendData(0xFF, x);
	}
}


