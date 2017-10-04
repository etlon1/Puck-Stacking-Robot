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
#include "motor.h"
#include "servo.h"
#include "color.h"
#include "ultrasonic.h"
#include "vl53l0x_functions.h"

#include "task1.h"
#include "task2.h"
#include "task3.h"

// Interrupt service routines //////////////////////////////////////////////////////////////////

// PID Controller routine
CY_ISR(PID_Timer_ISR) {
    Motor_PID_Adjust();
    PID_Timer_ReadStatusRegister();         // Clear interrupt register
}

/// Ultrasonic sensor routine
CY_ISR(Ultrasonic_ISR) {
    Ultrasonic_CalculateDistance();
    Ultrasonic_Timer_ReadStatusRegister();             // Clear interrupt register
}

// Front button routine
CY_ISR(Button_Front_Handler) {
    
    //Motor_MoveForward();
    
    run_task_1();
    
    Button_Front_ClearInterrupt(); 
}

// Back button routine
CY_ISR(Button_Back_Handler) {
    
    Button_Back_ClearInterrupt(); 
}
///////////////////////////////////////////////////////////////////////////////////////////////////


int main(void)
{   
    
    CyGlobalIntEnable;  // Enable global interrupts
    
    // Interrupt handlers
    PID_Timer_Int_StartEx(PID_Timer_ISR);
    Ultrasonic_Int_StartEx(Ultrasonic_ISR);
    Button_Front_Int_StartEx(Button_Front_Handler);
    Button_Back_Int_StartEx(Button_Back_Handler);
    
    // Initialization/startup code
    UART_Start();       // Initializes the UART (printing to terminal)
    I2C_Start();        // Initializes the I2C protocol for use with the VL53L0X laser sensors
    PWM_Buzzer_Start();
    
    Motor_Start();
    Servo_Start();
    Color_Start();
    Laser_Start();
    Ultrasonic_Start();

    
    for(;;)
    {   
        
    }
}


/* [] END OF FILE */
