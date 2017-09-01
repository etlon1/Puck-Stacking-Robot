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

cystatus Status;

char string[200];            // String outputed through the UART

int task = 0;

int32 leftCounter = 0;
int32 prevLeftCounter = 0;
int32 rightCounter = 0;
int32 prevRightCounter = 0;

int16 airTemp;                  // Assume air temperature is the same as the dire
float timeOfFlight = 0;        // Ultrasonic wave travel time
float soundSpeed;               // Value of sound speed based on the air temperature
float distanceTravelled = 0.0;  // Distance based on the time of flight
int speed;

CY_ISR(Button_Handler) {
    
    //UART_PutString("\t\t\tSpeed: ");
    
    //speed = PWM_ReadCompare();
    //speed = ((speed + 5) % 105);

    //sprintf(string, "%d\n", speed);      // Convert and store to string
    //UART_PutString(string);
    
    //PWM_WriteCompare(speed);
    
    
    
    /*
    task = (task + 1) % 5;
    
    CS_LED_TurnOff(1);
    CS_LED_TurnOff(2);
    CS_LED_TurnOff(3);
    
    if(task == 4) {
        CS_LED_TurnOn(1);
        CS_LED_TurnOn(2);
        CS_LED_TurnOn(3);
    }
    else
        CS_LED_TurnOn(task);*/
    
    Button_ClearInterrupt();
}

CY_ISR(Ultrasonic_ISR)
{
    Timer_ReadStatusRegister();             // Clear interrupt register
    
    Status = DieTemp_GetTemp(&airTemp);     // Get the air temperature (measured from the die temperature)
    soundSpeed = 331.3 + (0.606*airTemp);   // Calculate speed of sound based on the air temperature
    
    timeOfFlight = (float) (65535 - Timer_ReadCounter())/20000;     // Read the timer counter and calculate time
    distanceTravelled = soundSpeed*timeOfFlight;
    
    sprintf(string, "%f\n", distanceTravelled);      // Convert and store to string
    UART_PutString(string);                 // Display the distance on the terminal
    
}

void run_task_1() {
    int color = CS_GetColor();
    
    CS_LED_TurnOn(color);
    CyDelay(2000);
    CS_LED_TurnOff(color);
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Interrupt handlers
    Button_Int_StartEx(Button_Handler);
    Ultrasonic_Int_StartEx(Ultrasonic_ISR);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();       // Initializes the UART (for debugging) 
    ADC_Start();
    Timer_Start();      // Initializes the timer for the ultrasonic
    QuadDec_Left_Start();
    QuadDec_Right_Start();
    PWM_Right_Forward_Start();
    PWM_Left_Forward_Start();
    
    for(;;)
    {   
        if(leftCounter < 50000 && rightCounter < 50000) {
            PWM_Right_Forward_WriteCompare(60);
            PWM_Left_Forward_WriteCompare(60);
        }
        else {
            PWM_Right_Forward_WriteCompare(0);

        }
        /*
        while( Echo_Read() == 0 ) {
            TrigReg_Write(1);   // Set register to 1, triggering the ultrasonic and reseting the timer
            CyDelayUs(10);      // Trigger signal must be 10us
            TrigReg_Write(0);   // Stop ultrasonic trigger signal
        }
        
        if(distanceTravelled < 5) {
            PWM_WriteCompare(0);
        }*/
                
        
        
        /*
        leftCounter = QuadDec_Left_GetCounter();
        rightCounter = QuadDec_Right_GetCounter();
        
        if((leftCounter != prevLeftCounter) || (rightCounter != prevRightCounter)) {   
            sprintf(string, "Left: %ld, \tRight: %ld\n", leftCounter, rightCounter);      // Convert and store to string
            UART_PutString(string);                 // Display the distance on the terminal
        }
        
        prevLeftCounter = leftCounter;
        prevRightCounter = rightCounter;*/

            
        CyDelay(100);
    }
}

/* [] END OF FILE */
