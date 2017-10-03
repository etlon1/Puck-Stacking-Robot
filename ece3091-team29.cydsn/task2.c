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
#include "task2.h"

extern float ultrasonicDistance;

void run_task_2() {
    UART_PutString("Running task 2\n");
    
    Motor_PID_Reset();
    Motor_MoveForward();
    
    ultrasonicDistance = 1000;
    
    while( ultrasonicDistance > 420 || QuadDec_Right_GetCounter() < 5000) {
        Ultrasonic_ReadDistance(1);
    }
    
    Motor_Stop();
    CyDelay(100);
    Motor_PID_Reset();
    
    Motor_TurnLeft(181);
    
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
