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

#ifndef _MOTOR_H_
#define _MOTOR_H_
    
#include "project.h"

void Motor_Start();

void Motor_MoveForward();
void Motor_MoveBackwards();

void Motor_TurnRight(float degrees);
void Motor_TurnLeft(float degrees);

void Motor_Stop();

void Motor_PID_Adjust(int rightDirection, int leftDirection);
void Motor_PID_Reset();

int Motor_LimitCompare(int compare);

#endif

/* [] END OF FILE */
