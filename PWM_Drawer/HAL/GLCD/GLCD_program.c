/**
 ******************************************************************************
 * @file    GLCD_program.c
 * @author  Salma Faragalla
 * @ brief  GLCD module driver.
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "BIT_MATH.h"
#include "STM32F103.h"

#include "../../MCAL/GPIO/GPIO_interface.h"

#include "GLCD_interface.h"
#include "GLCD_config.h"
#include "GLCD_font.h"

/* Private Function Prototypes-------------------------------------------------*/

/**
 * @brief Creates a simple delay using a busy-wait loop.
 * @param t : The delay duration in arbitrary units
 * @retval None
 */
static void Delay(u8 t);
/**
 * @brief Sends a single byte of data to the GLCD display
 * @param data : Byte of data to be sent to the LCD.
 * @retval None
 */
static void GLCD_Send(u8 data);

/**
 * @brief Sends a command byte to the GLCD display
 * @param data : Command byte to be sent to the LCD.
 * @retval None
 */
static void GLCD_SendCommand(u8 command);

/**
 * @brief Sends a data byte to the GLCD display, selecting the left or right section based on the provided x coordinate
 * @param data : Data byte to be sent to the LCD.
 * @param x : X coordinate 0-127)
 * @retval None
 */
static void GLCD_SendData(u8 data, u8 x);

/**
 * @brief Sets the cursor position to a specific line
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
static void GLCD_GoToLine(GLCD_LineNum_t y);

/**
 * @brief Sets the cursor position to a specific column / X coordinate
 * @param  x : x coordinate (0-127)
 * @retval None
 */
static void GLCD_GoToColumn(u8 x);

/**
 * @brief Sets the cursor position to a specific column/ X coordinate and a specific line
 * @param  x : x coordinate (0-127)
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
static void GLCD_GoTo_Col_Line(u8 x, u8 line);
/**
 * @brief  Reverses a given string
 * @param  Pointer to string
 * @param  length : length of the string
 * @retval None
 */
static void GLCD_reverseString(char* str, int length) ;

/* Private Functions ---------------------------------------------------------*/

/**
 * @brief Creates a simple delay using a busy-wait loop.
 * @param t : The delay duration in arbitrary units
 * @retval None
 */
static void Delay(u8 t)
{
	t *= 10;
	while (t)
	{
		t--;
	}
}

/**
 * @brief Sends a single byte to the GLCD display
 * @param data : Byte to be sent to the LCD.
 * @retval None
 */
static void GLCD_Send(u8 data)
{
	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, GPIO_PIN_LOW); // Set Enable pin to low to initiate data transmission
	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_RW_PIN, GPIO_PIN_LOW); // Set Read/Write pin to low for write operation
	Delay(T);
	GPIO_SetPortValue(GLCD_DATA_PORT, GPIO_PORT_LOW); // Clear the Data port to prepare for new data
	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, GPIO_PIN_HIGH); // Pulse the Enable pin to send the data
	Delay(T);

	// Set individual data bits on the Data port based on the provided data byte
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D0_PIN,GET_BIT(data,0));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D1_PIN,GET_BIT(data,1));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D2_PIN,GET_BIT(data,2));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D3_PIN,GET_BIT(data,3));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D4_PIN,GET_BIT(data,4));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D5_PIN,GET_BIT(data,5));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D6_PIN,GET_BIT(data,6));
	GPIO_SetPinValue(GLCD_DATA_PORT, GLCD_D7_PIN,GET_BIT(data,7));

	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, GPIO_PIN_LOW); // Clear the Enable pin
}

/**
 * @brief Sends a command byte to the GLCD display
 * @param data : Command byte to be sent to the LCD.
 * @retval None
 */
static void GLCD_SendCommand(u8 command)
{
	// Set Data/Instruction  pin to low for command mode
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_DI_PIN, GPIO_PIN_LOW);
	Delay(T);

	// Select both LCD controller chips (CS1 and CS2)
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_HIGH);
	Delay(T);
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_PIN_HIGH);
	Delay(T);

	// Send command to GLCD
	GLCD_Send(command);
}

/**
 * @brief Sends a data byte to the GLCD display, selecting the left or right section based on the provided x coordinate
 * @param data : Data byte to be sent to the LCD.
 * @param x : X coordinate 0-127)
 * @retval None
 */
void GLCD_SendData(u8 data, u8 x)
{
	// Set Data/Instruction  pin to high for data mode
	GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_DI_PIN, GPIO_PIN_HIGH);
	Delay(T);

	// Check if the X coordinate is within the left half of the screen
	if (x < GLCD_SCREEN_HALF_WIDTH)
	{
		// Select left section controller chip (CS1) and deselect right section (CS2)
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_HIGH);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_PIN_LOW);
		Delay(T);
		// Send data to GLCD
		GLCD_Send(data);
	}

	else
	{
		// Select right section controller chip (CS2) and deselect left section (CS1)
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_LOW);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_PIN_HIGH);
		Delay(T);
		// Send data to GLCD
		GLCD_Send(data);
	}
}

/**
 * @brief Sets the cursor position to a specific line
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
static void GLCD_GoToLine(GLCD_LineNum_t y)
{
	Delay(T);
	y = (y | 0xB8) & 0xBF; // Command from data sheet
	GLCD_SendCommand(y);
}

/**
 * @brief  Sets the cursor position to a specific column / X coordinate
 * @param  x : x coordinate (0-127)
 * @retval None
 */
static void GLCD_GoToColumn(u8 x)
{
	Delay(T);
	// Check if the X coordinate is within the left half of the screen
	if (x < GLCD_SCREEN_HALF_WIDTH)
	{
		x = (x | 0x40) & 0x7F; // Command from data sheet

		//Set Data/Instruction  pin to low for command mode
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_DI_PIN, GPIO_PIN_LOW);
		Delay(T);

		// Select left section controller chip (CS1) and deselect right section (CS2)
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_HIGH);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_PIN_LOW);
		Delay(T);

		// Send command to GLCD
		GLCD_Send(x);
	}
	else
	{
		x -= GLCD_SCREEN_HALF_WIDTH; // Adjust x for right section
		x = (x | 0x40) & 0x7F;  // command from data sheet

		// Select right section controller chip (CS2) and deselect left section (CS1)
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_DI_PIN, GPIO_PIN_LOW);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, GPIO_PIN_LOW);
		Delay(T);
		GPIO_SetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, GPIO_PIN_HIGH);
		Delay(T);

		// Send command to GLCD
		GLCD_Send(x);
	}
}

/**
 * @brief  Sets the cursor position to a specific column/ X coordinate and a specific line
 * @param  x : x coordinate (0-127)
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
static void GLCD_GoTo_Col_Line(u8 x, GLCD_LineNum_t line)
{
	Delay(T);
	GLCD_GoToLine(line);
	GLCD_GoToColumn(x);
}

/**
 * @brief  Reverses a given string
 * @param  Pointer to string
 * @param  length : length of the string
 * @retval None
 */
static void GLCD_reverseString(char* str, int length)
{
	 // Initialize start and end indices for swapping characters
	    int start = 0;
	    int end = length - 1;

	    // Iterate while the start index is less than the end index
	    while (start < end)
	    {
	    	// Swap the characters at start and end indices
	        char temp = str[start];
	        str[start] = str[end];
	        str[end] = temp;

	        // Move start and end indices towards the middle for the next iteration
	        start++;
	        end--;
	    }
}

/*Public Functions -------------------------------------------------*/
/**
 * @brief  Initializes GLCD display
 * @param  None
 * @retval None
 */
void GLCD_Init(void)
{
	/* Enable GPIO Clock*/
	RCC_GPIOA_CLK_EN();
	RCC_GPIOB_CLK_EN();

	/* Set GLCD ports direction and speed */
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

/**
 * @brief  Prints a single character on  GLCD display
 * @param  x : X coordinate
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */

void GLCD_PrintChar(u8 data, u8 x, GLCD_LineNum_t y)
{
	int i,j;
	i=0;
	// Calculate the end column index for the character (considering its width)
	j = i + GLCD_FONT_CHAR_WIDTH;
	// Check if the X coordinate would exceed the screen width after printing the character
	if (x >= (GLCD_SCREEN_WIDTH - GLCD_FONT_CHAR_WIDTH))
	{
		++y; // Move to the next line
		x=0; // Start printing at the beginning of the next line
	}
	// Loop through each pixel column (width) of the character
	for (i=0; i <= j; i++)
	{
		GLCD_GoTo_Col_Line(x, y); // Set the cursor
		GLCD_SendData(Font[data][i], x); // Get the pixel data for the current column from the font array based on the character and column index
		++x;					  // Move to the next pixel column for the next iteration
	}

}

/**
 * @brief  Prints a string on  GLCD display
 * @param  x : X coordinate
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
void GLCD_PrintString(string str, u8 x, GLCD_LineNum_t y)
{
	while (*str != 0)
	{
		// Check if the X coordinate would exceed the screen width after printing the character
		if (x + GLCD_FONT_CHAR_WIDTH >=GLCD_SCREEN_WIDTH)
		{
			++y; // Move to the next line
			x=0; // Start printing at the beginning of the next line
		}

		// Print the current character at the current position
		GLCD_PrintChar(*str, x, y);

		// Move to the next character in the string and the next column for printing
		x+=GLCD_FONT_CHAR_WIDTH;
		str++;
	}
}
/**
 * @brief  Prints an unsigned 32-bit integer as a string on the GLCD display.
 * @param  x : X coordinate
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
void GLCD_PrintNum(u32 num , u8 x , u8 y)
{
		// Temporary array to store the converted string (maximum 10 digits)
		u8 str [10];

		//Handle special case: zero
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

	    // Calculate the number of digits
	    while (n != 0)
	    {
	        len++;
	        n /= 10;
	    }

	    // Convert digits from least significant to most significant (reverse order)
	    for (i = 0; i < len; i++)
	    {
	        rem = num % 10;
	        num /= 10;
	        str[len - (i + 1)] = rem + '0'; // // Convert digit to ASCII character
	    }

	    // Terminate the string
	    str[len] = '\0';

	    // Print converted string to GLCD
	    GLCD_PrintString(str, x, y);
	    return;

}

/**
 * @brief  Prints an float 32-bit integer as a string on the GLCD display.
 * @param  x : X coordinate
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */
void GLCD_PrintFloat(f32 num , u8 x , u8 y)
{
	  // Temporary array to store the converted string
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


/**
 * @brief  Clears a specific line on the GLCD display
 * @param  y : line number on GLCD (GLCD_LINE_0 to GLCD_LINE_7)
 * @retval None
 */

void GLCD_ClearLine(GLCD_LineNum_t line)
{
	Delay(T);
	int i;
	// Loop through all pixels on the line, sending data byte 0 for clearing
	// Left half of the line
	GLCD_GoTo_Col_Line(0, line);
	for (i = 0; i < 64; i++)
	{
		GLCD_SendData(0, 0);	 // Print 0 for Delete Left section
		GLCD_SendData(0, 64); // Print 0 for Delete Right section
	}

	// Right half of the line
	GLCD_GoTo_Col_Line(64, line); // At start of line of right side
	for (i = 0; i < 64; i++)
	{
		GLCD_SendData(0, 0);	 // Print 0 for Delete Left section
		GLCD_SendData(0, 64); // Print 0 for Delete Right section
	}

}

/**
 * @brief  Draws a horizontal line on GLCD display
 * @param  x1 : The starting X coordinate (column) of the line.
 * @param  x2 : The ending X coordinate (column) of the line.
 * @retval None
 */

void GLCD_DrawHLine(u8 x1, u8 x2, GLCD_LineNum_t y)
{
	for (char x=x1;x<=x2;x++)
	{
		GLCD_GoTo_Col_Line (x,y);
		GLCD_SendData(0x01, x);
	}

}
/**
 * @brief  Draws a horizontal line on GLCD display
 * @param  y1 : The starting line number of the line..
 * @param  y2 : The ending line number of the line.
 * @param  x : x coordinate
 * @retval None
 */
void GLCD_DrawVLine(GLCD_LineNum_t y1, GLCD_LineNum_t y2, u8 x)
{
	for (char y=y1;y<y2;y++)
	{
		GLCD_GoTo_Col_Line (x,y);
		GLCD_SendData(0xFF, x);
	}
}


