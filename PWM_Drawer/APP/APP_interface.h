/*
 * APP_interface.h
 */

#ifndef APP_INTERFACE_H_
#define APP_INTERFACE_H_

#include "STD_TYPES.h"

void APP_Init(void);

void APP_IC_Start(void);
void APP_PWM_Start(u32 dutyCycle, u32 frequency);
void APP_GLCD_Update();

#endif /* APP_INTERFACE_H_ */
