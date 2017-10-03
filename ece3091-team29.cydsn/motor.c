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
#include "motor.h"

#define MOTOR_SPEED 3500
#define MOTOR_MIN_SPEED 3000
#define MOTOR_MAX_SPEED 10000
#define MOTOR_TURN_VALUE_360 17800
#define MOTOR_KP 10
#define MOTOR_KI 1 //1
#define MOTOR_KD 100

int error;
int prevError;
int derivative;
int integral;

extern int rightWheelDirection;
extern int leftWheelDirection;

char motor_string[100];

void Motor_Start() {
    PWM_Right_Forward_Start();
    PWM_Left_Forward_Start();
    PWM_Right_Reverse_Start();
    PWM_Left_Reverse_Start();
    
    QuadDec_Right_Start();
    QuadDec_Left_Start();
}

void Motor_MoveForward() {
    Motor_Stop();
    
    PWM_Right_Forward_WriteCompare(MOTOR_SPEED);
    PWM_Left_Forward_WriteCompare(MOTOR_SPEED);
    
    rightWheelDirection = 1;
    leftWheelDirection = 1;
}

void Motor_MoveBackwards() {
    Motor_Stop();
    
    rightWheelDirection = -1;
    leftWheelDirection = -1;
    
    PWM_Right_Reverse_WriteCompare(MOTOR_SPEED);
    PWM_Left_Reverse_WriteCompare(MOTOR_SPEED);
}

void Motor_TurnLeft(float degrees) {
    float angleTicks = MOTOR_TURN_VALUE_360*degrees/360;
    
    PWM_Right_Forward_WriteCompare(3000);
    PWM_Left_Reverse_WriteCompare(3000);
    
    rightWheelDirection = 1;
    leftWheelDirection = -1;  
    
    int rightFinish = 0;
    int leftFinish = 0;
    
    while(!rightFinish || !leftFinish) {
        if( QuadDec_Right_GetCounter() >= angleTicks ) {
            PWM_Right_Forward_WriteCompare(0);
            rightFinish = 1;
            rightWheelDirection = 0;
        }
        if( QuadDec_Left_GetCounter() <= -angleTicks ) {
            PWM_Left_Reverse_WriteCompare(0);
            leftFinish = 1;
            leftWheelDirection = 0;
        }
    }
}

void Motor_TurnRight(float degrees) {
    float angleTicks = MOTOR_TURN_VALUE_360*degrees/360;
    
    PWM_Right_Reverse_WriteCompare(3000);
    PWM_Left_Forward_WriteCompare(3000);
    
    rightWheelDirection = -1;
    leftWheelDirection = 1;
    
    int rightFinish = 0;
    int leftFinish = 0;
    
    while(!rightFinish || !leftFinish) {
        if( QuadDec_Right_GetCounter() <= -angleTicks ) {
            PWM_Right_Reverse_WriteCompare(0);
            rightFinish = 1;
            rightWheelDirection = 0;
        }
        if( QuadDec_Left_GetCounter() >= angleTicks ) {
            PWM_Left_Forward_WriteCompare(0);
            leftFinish = 1;
            UART_PutString("Left Finished\n");
            leftWheelDirection = 0;
        }
    }
}

void Motor_Stop() {
    PWM_Right_Forward_WriteCompare(0);
    PWM_Left_Forward_WriteCompare(0);
    PWM_Right_Reverse_WriteCompare(0);
    PWM_Left_Reverse_WriteCompare(0);
    
    rightWheelDirection = 0;
    leftWheelDirection = 0;
}

void Motor_PID_Reset() {
    error = 0;
    prevError = 0;
    derivative = 0;
    integral = 0;
    
    QuadDec_Right_SetCounter(0);
    QuadDec_Left_SetCounter(0);
}

void Motor_PID_Adjust(int rightDirection, int leftDirection) {
    
    if(rightWheelDirection != 0 && leftWheelDirection != 0) {
        int rightCompare = 0;
        int leftCompare = 0;
        float correction;
        
        error = (rightDirection)*QuadDec_Right_GetCounter() - (leftDirection)*QuadDec_Left_GetCounter();
        derivative = error - prevError;
        integral += error;

        correction = MOTOR_KP*error + MOTOR_KI*integral + MOTOR_KD*derivative;
        
        if( rightDirection > 0 ) {
            rightCompare = PWM_Right_Forward_ReadCompare();
        }
        else if( rightDirection < 0 ) {
            rightCompare = PWM_Right_Reverse_ReadCompare();
        }
        
        if( leftDirection > 0 ) {    
            leftCompare = PWM_Left_Forward_ReadCompare();
        }
        else if( leftDirection < 0 ) {
            leftCompare = PWM_Left_Reverse_ReadCompare();
        }
        
        rightCompare -= correction/2;
        leftCompare += correction/2;
        
        rightCompare = Motor_LimitCompare(rightCompare);
        leftCompare = Motor_LimitCompare(leftCompare);
        
        if( rightDirection > 0 ) {
            PWM_Right_Forward_WriteCompare(rightCompare);
        }
        else if( rightDirection < 0 ) {
            PWM_Right_Reverse_WriteCompare(rightCompare);
        }
        
        if( leftDirection > 0 ) {    
            PWM_Left_Forward_WriteCompare(leftCompare);
        }
        else if( leftDirection < 0 ) {
            PWM_Left_Reverse_WriteCompare(leftCompare);
        }
        
        prevError = error;
        
        //sprintf(motor_string, "Left:\t%d,\tRight:\t%d,\tError:\t%d,\tCorrection:\t%.2f\n", leftCompare, rightCompare, error, correction);      // Convert and store to string
        //UART_PutString(motor_string); 
    }
}

int Motor_LimitCompare(int compare) {
    if(compare < MOTOR_MIN_SPEED) {
        return MOTOR_MIN_SPEED;
    }
    else if(compare > MOTOR_MAX_SPEED) {
        return MOTOR_MAX_SPEED;
    }
    else {
        return compare;
    }
}

/* [] END OF FILE */
