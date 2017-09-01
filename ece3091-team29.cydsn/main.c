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
#include "project.h"
#include "color_sensor.h"
#include <stdio.h>

void run_task_1();
void run_task_2();
void run_task_3();
void run_task_4();

char string[200];            // String outputed through the UART

int task = 0;

cystatus Status;
int16 airTemp;                  // Assume air temperature is the same as the dire
float timeOfFlight = 0;        // Ultrasonic wave travel time
float soundSpeed;               // Value of sound speed based on the air temperature
float distanceTravelled = 0.0;  // Distance based on the time of flight
int speed;
int frontDistance = 100;
int backDistance = 100;

CY_ISR(Ultrasonic_ISR)
{
    //UART_PutString("hello\n");
    Timer_ReadStatusRegister();             // Clear interrupt register
    
    Status = DieTemp_GetTemp(&airTemp);     // Get the air temperature (measured from the die temperature)
    soundSpeed = 331.3 + (0.606*airTemp);   // Calculate speed of sound based on the air temperature
    
    timeOfFlight = (float) (65535 - Timer_ReadCounter())/20000;     // Read the timer counter and calculate time
    distanceTravelled = soundSpeed*timeOfFlight;
    
    //sprintf(string, "%f\n", distanceTravelled);      // Convert and store to string
    //UART_PutString(string);                 // Display the distance on the terminal
    
}


// Interrupt routine called when RUN TASK BUTTON (PSoC) is pressed
CY_ISR(PSoC_Button_Handler) {
    
    switch(task) {
        case 1:
        UART_PutString("Running task 1\n");
        //run_task_1();
        break;
        
        case 2:
        UART_PutString("Running task 2\n");
        //run_task_2();
        break;
        
        case 3:
        UART_PutString("Running task 3\n");
        //run_task_3();
        break;
        
        case 4:
        UART_PutString("Running task 4\n");
        // Color sensor will run until RESET BUTTON is pressed
        while(1) {
            run_task_4();
        }
        break;
        
        default:
        UART_PutString("No task selected\n");
    }
    task = 0;       // Reset task option
    
    PSoC_Button_ClearInterrupt();
}

// Interrupt routine called when TASK SELECTION BUTTON (external) is pressed
CY_ISR(Ext_Button_Handler) {
    
    task = (task + 1) % 5;
    
    sprintf(string, "Selected task %d\n", task);
    UART_PutString(string);
    
    CS_LED_TurnOffAll();
    
    if(task == 4) {
        CS_LED_TurnOnAll();
    }
    else
        CS_LED_TurnOn(task);
        
    Ext_Button_ClearInterrupt();
}

// TASK 1 - Reverse to base //////////////////////////////////////////////////////////////////////////////////////////////////
void run_task_1() {
    
    
        
    
    
}
// END OF TASK 1 /////////////////////////////////////////////////////////////////////////////////////////////////////////////


// TASK 2 - Turn bot by 180 degrees and return to base ///////////////////////////////////////////////////////////////////////
void run_task_2() {
    
    
}
// END OF TASK 2 /////////////////////////////////////////////////////////////////////////////////////////////////////////////


// TASK 3 - Going around the block and return base ///////////////////////////////////////////////////////////////////////////
void run_task_3() {
    
    
}
// END OF TASK 3 //////////////////////////////////////////////////////////////////////////////////////////////////////////////


// TASK 4 - Color sensing /////////////////////////////////////////////////////////////////////////////////////////////////////
// NOTE: make sure you the sensor is above the puck before the next reading starts
void run_task_4() {
    int color = CS_GetColor();
    
    CS_LED_TurnOn(color);
    CyDelay(3000);
    CS_LED_TurnOff(color);
}
// END OF TASK 4 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{   
    CyGlobalIntEnable;  // Enable global interrupts
    
    // Interrupt handlers
    PSoC_Button_Int_StartEx(PSoC_Button_Handler);
    Ext_Button_Int_StartEx(Ext_Button_Handler);
    Ultrasonic_Int_StartEx(Ultrasonic_ISR);
    
    
    // Initialization/startup code
    UART_Start();       // Initializes the UART (printing to terminal) 
    ADC_Start();        // Photodiode analog to digital converter
    Timer_Start();      // Initializes the timer for the ultrasonic
    QuadDec_Left_Start();
    QuadDec_Right_Start();
    PWM_Right_Forward_Start();
    PWM_Left_Forward_Start();
    PWM_Right_Reverse_Start();
    PWM_Left_Reverse_Start();
    
    PWM_Left_Forward_WriteCompare(40);
    PWM_Right_Forward_WriteCompare(40);
    for(;;)
    {     
        
    
    while((frontDistance > 17) && (QuadDec_Left_GetCounter() < 15000) && (QuadDec_Right_GetCounter() < 15000)){
            
        if( Echo_Front_Read() == 0 ) {
           
            uint8 regValWrite = 1;
            uint8 regValStop = 0;
            TrigReg_Write(regValWrite);   // Set register to 1, triggering the ultrasonic and reseting the timer
            
            CyDelayUs(10);      // Trigger signal must be 10us
            TrigReg_Write(regValStop);   // Stop ultrasonic trigger signal
            
            CyDelay(100);
            frontDistance = distanceTravelled;
           
            UART_PutString(string);
        }
        
  
    }
    PWM_Left_Forward_WriteCompare(0);
    PWM_Right_Forward_WriteCompare(0);
    CyDelay(100);
    PWM_Left_Reverse_WriteCompare(40);
    PWM_Right_Reverse_WriteCompare(40);
        
    while((backDistance > 5)){
        
        sprintf(string, "Back Distance: %f\t\tLeftC: %ld\t\tRightC: %ld\t\t\n", distanceTravelled,QuadDec_Left_GetCounter(),QuadDec_Right_GetCounter() );

            
        if( Echo_Back_Read() == 0 ) {
           
            uint8 regValWrite = 2;
            uint8 regValStop = 0;
            TrigReg_Write(regValWrite);   // Set register to 1, triggering the ultrasonic and reseting the timer
            
            CyDelayUs(10);      // Trigger signal must be 10us
            TrigReg_Write(regValStop);   // Stop ultrasonic trigger signal
            
            CyDelay(100);
            backDistance = distanceTravelled;
            
            UART_PutString(string);
        }
    
    PWM_Left_Reverse_WriteCompare(0);
    PWM_Right_Reverse_WriteCompare(0);
    

    }
    }
        
        
}


/* [] END OF FILE */
