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

#include "servo.h"

#define SERVO_GRIPPER_DELAY 500
#define SERVO_ARM_DELAY 1000

void Servo_Start() {
    
    PWM_Arm_Start();
    PWM_Gripper_Start();
}

void Servo_GripperGrab() {
    
    PWM_Gripper_WriteCompare(2000);
    CyDelay(SERVO_GRIPPER_DELAY);
    Servo_ArmMove(5);
}

void Servo_GripperRelease() {
    
    PWM_Gripper_WriteCompare(800);
    CyDelay(SERVO_GRIPPER_DELAY);
}

void Servo_ArmMove(int puckNumber) {
    
    switch(puckNumber) {
        case 1:
            PWM_Arm_WriteCompare(800);
            break;
        case 2:
            PWM_Arm_WriteCompare(1000);
            break;
        case 3:
            PWM_Arm_WriteCompare(1200);
            break;
        case 4:
            PWM_Arm_WriteCompare(1400);
            break;
        case 5:
            PWM_Arm_WriteCompare(1600);
            break;
    }
    CyDelay(SERVO_ARM_DELAY);
}

void Servo_Stack(int puckNumber) {
    
    Servo_ArmMove(puckNumber);
    Servo_Release();
}

/* [] END OF FILE */
