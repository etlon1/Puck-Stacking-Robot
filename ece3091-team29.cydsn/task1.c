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

char string[100];

void run_task_1() {
    UART_PutString("Running task 1\n");
    
    Motor_PID_Reset();
    Motor_MoveForward();
    
//    Motor_PID_Reset();
//    Motor_TurnRight(90);
//    Motor_Stop();
//    CyDelay(10000);
//    
//    Motor_PID_Reset();
//    Motor_TurnLeft(90);
//    Motor_Stop();
//    CyDelay(1000);
//
//    Motor_PID_Reset();
//    Motor_TurnRight(90);
//    Motor_Stop();
//    CyDelay(1000);
//    
//    Motor_PID_Reset();
//    Motor_TurnRight(90);
//    Motor_Stop();
//    CyDelay(1000);

}

/* [] END OF FILE */
