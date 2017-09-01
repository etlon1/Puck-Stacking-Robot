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
#include <stdio.h>
#include "project.h"

int CS_GetColor();
void CS_LED_TurnOn(int color);
void CS_LED_TurnOff(int color);
void CS_LED_TurnOnAll();
void CS_LED_TurnOffAll();

int _CS_GetSample();
int _CS_GetMultipleSamples();
int _CS_IsSampleGood(int red, int green, int blue, int color);

/* [] END OF FILE */
