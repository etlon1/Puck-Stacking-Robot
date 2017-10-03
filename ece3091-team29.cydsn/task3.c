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
#include "task3.h"

extern float ultrasonicDistance;

void run_task_3() {
    UART_PutString("Running task 3\n");
    
    float stopDistance = 250;
    
    Motor_PID_Reset();
    Motor_MoveForward();
    
    ultrasonicDistance = 1000;
    
    while( ultrasonicDistance > 125 || QuadDec_Right_GetCounter() < 5000 ) {
        Ultrasonic_ReadDistance(1);
    }
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    Motor_TurnRight(90);
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    // Part 2
    
    Motor_MoveForward();
    
    ultrasonicDistance = 1000;
    
    while( ultrasonicDistance > stopDistance ) {
        Ultrasonic_ReadDistance(1);
    }
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    Motor_TurnLeft(90);
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    // Part 3
    
    Motor_MoveForward();
    
    ultrasonicDistance = 1000;
    
    while( ultrasonicDistance > stopDistance ) {
         Ultrasonic_ReadDistance(1);
    }
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    Motor_TurnLeft(90);
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    Motor_MoveForward();
    
    ultrasonicDistance = 1000;
    
    while( ultrasonicDistance > stopDistance ) {
        
         Ultrasonic_ReadDistance(1);
    }
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    Motor_TurnLeft(90);
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    Motor_MoveForward();
    
    ultrasonicDistance = 1000;
    
    while( ultrasonicDistance > stopDistance ) {
        
         Ultrasonic_ReadDistance(1);
    }
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    Motor_TurnLeft(90);
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    
    //
    
    Motor_MoveForward();
    
    ultrasonicDistance = 1000;
    
    while( ultrasonicDistance > 565 ) {
        
         Ultrasonic_ReadDistance(1);
    }
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    Motor_TurnRight(90);
    
    //
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    Motor_MoveForward();
    
    ultrasonicDistance = 1000;
    
    while( ultrasonicDistance > 50 ) {
        
         Ultrasonic_ReadDistance(1);
    }
    
    Motor_Stop();
}

/* [] END OF FILE */
