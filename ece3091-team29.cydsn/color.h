/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef _COLOR_SENSOR_H_
#define _COLOR_SENSOR_H_
    
#include <stdio.h>
#include "project.h"

int Color_GetColor();
void Color_Start();
void Color_LED_TurnOn(int color);
void Color_LED_TurnOff(int color);
void Color_LED_TurnOnAll();
void Color_LED_TurnOffAll();

int Color_GetSample();
int Color_GetMultipleSamples();
int Color_IsSampleGood(int red, int green, int blue, int color);

#endif

/* [] END OF FILE */
