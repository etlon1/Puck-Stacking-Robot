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

#ifndef _SERVO_H_
#define _SERVO_H_
    
#include "project.h"

void Servo_Start();
void Servo_GripperGrab();
void Servo_GripperRelease();
void Servo_ArmMove(int puckNumber);
void Servo_Stack(int puckNumber);

#endif

/* [] END OF FILE */
