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

#include "task1.h"
#include "color.h"

#define PI 3.14159265

char string[100];

extern float ultrasonicDistance;

void run_task_1() {
    UART_PutString("Running task 1\n");
    
    float rightDistance;
    float leftDistance;
    float difference;
    double angle;
    int i;
    
    difference = 100;
    
    Motor_PID_Reset();
    Motor_MoveForward();
   
    ultrasonicDistance = 1000;
    while(ultrasonicDistance > 250 || QuadDec_Right_GetCounter() < 1000) {
        Ultrasonic_ReadDistance(1);
        CyDelay(10);
    }
    Motor_Stop();
    Motor_PID_Reset();
        
    while( fabs( difference ) > 3 ) {
        Ultrasonic_ReadDistance(1);
        CyDelay(10);
        rightDistance = ultrasonicDistance;
        sprintf(string, "Right: %f\n", ultrasonicDistance);
        UART_PutString(string);
        for(i = 0; i < 9; i++) {
            Ultrasonic_ReadDistance(1);
            CyDelay(10);
            sprintf(string, "Right: %f\n", ultrasonicDistance);
            UART_PutString(string);
            rightDistance = ( rightDistance + ultrasonicDistance )/2;
        }

        Ultrasonic_ReadDistance(2);
        CyDelay(10);
        leftDistance = ultrasonicDistance;
        sprintf(string, "Left: %f\n", ultrasonicDistance);
        UART_PutString(string);
        for(i = 0; i < 9; i++) {
            Ultrasonic_ReadDistance(2);
            CyDelay(10);
            sprintf(string, "Left: %f\n", ultrasonicDistance);
            UART_PutString(string);
            leftDistance = ( leftDistance + ultrasonicDistance )/2;
        }
        
        difference = rightDistance - leftDistance;
        angle = atan(difference/120)*(180/PI);
        
        sprintf(string, "Right: %f\t Left: %f\tDifference: %f\tAngle: %f\n", rightDistance, leftDistance, difference, angle);
        UART_PutString(string);
        
        if(angle < 0) {
            Motor_TurnRight(-angle/1.5);
        }
        else if(angle > 0) {
            Motor_TurnLeft(angle/1.5);
        }
        CyDelay(100);
        Motor_PID_Reset();
    }

    CyDelay(100);  
    Motor_TurnLeft(90);
    Motor_PID_Reset();
    CyDelay(100);
    
    
    
    
    difference = 100;
        
    Motor_PID_Reset();
    Motor_MoveForward();
   
    ultrasonicDistance = 1000;
    while(ultrasonicDistance > 203 || QuadDec_Right_GetCounter() < 1000) {
        Ultrasonic_ReadDistance(1);
        CyDelay(10);
    }
    Motor_Stop();
    Motor_PID_Reset();
        
    while( fabs( difference ) > 3 ) {
        Ultrasonic_ReadDistance(1);
        CyDelay(10);
        rightDistance = ultrasonicDistance;
        sprintf(string, "Right: %f\n", ultrasonicDistance);
        UART_PutString(string);
        for(i = 0; i < 9; i++) {
            Ultrasonic_ReadDistance(1);
            CyDelay(10);
            sprintf(string, "Right: %f\n", ultrasonicDistance);
            UART_PutString(string);
            rightDistance = ( rightDistance + ultrasonicDistance )/2;
        }

        Ultrasonic_ReadDistance(2);
        CyDelay(10);
        leftDistance = ultrasonicDistance;
        sprintf(string, "Left: %f\n", ultrasonicDistance);
        UART_PutString(string);
        for(i = 0; i < 9; i++) {
            Ultrasonic_ReadDistance(2);
            CyDelay(10);
            sprintf(string, "Left: %f\n", ultrasonicDistance);
            UART_PutString(string);
            leftDistance = ( leftDistance + ultrasonicDistance )/2;
        }
        
        difference = rightDistance - leftDistance;
        angle = atan(difference/120)*(180/PI);
        
        sprintf(string, "Right: %f\t Left: %f\tDifference: %f\tAngle: %f\n", rightDistance, leftDistance, difference, angle);
        UART_PutString(string);
        
        if(angle < 0) {
            Motor_TurnRight(-angle/1.5);
        }
        else if(angle > 0) {
            Motor_TurnLeft(angle/1.5);
        }
        CyDelay(100);
        Motor_PID_Reset();
    }

    CyDelay(100);  
    Motor_TurnLeft(90);
    Motor_PID_Reset();
    CyDelay(100);
    

}

/* [] END OF FILE */
