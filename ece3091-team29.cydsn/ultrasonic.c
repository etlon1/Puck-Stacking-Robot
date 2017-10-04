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
#include "ultrasonic.h"

char string[100];

float ultrasonicDistance = 0;  // Distance based on the time of flight

void Ultrasonic_Start() {
    Ultrasonic_Timer_Start();      // Initializes the timer for the ultrasonic
    DieTemp_Start();
}

void Ultrasonic_CalculateDistance() {
    
    int16 airTemp;          // Assume air temperature is the same as the dire
    float timeOfFlight;     // Ultrasonic wave travel time
    float soundSpeed;       // Value of sound speed based on the air temperature
    
    DieTemp_GetTemp(&airTemp);     // Get the air temperature (measured from the die temperature)
    soundSpeed = 331.3 + (0.606*airTemp);   // Calculate speed of sound based on the air temperature
    
    timeOfFlight = (float) (65535 - Ultrasonic_Timer_ReadCounter())/2000;     // Read the timer counter and calculate time
    ultrasonicDistance = soundSpeed*timeOfFlight;                    // Calculate the distance in mm
    
//    sprintf(string, "%f\n", ultrasonicDistance);     // Convert and store to string
//    UART_PutString(string);                         // Display the distance on the terminal
}

float Ultrasonic_ReadDistance(int sensorNumber) {
    TrigReg_Write(sensorNumber);    // Set register to 1, triggering the ultrasonic and reseting the timer
    CyDelayUs(10);                  // Trigger signal must be 10us
    TrigReg_Write(0);               // Stop ultrasonic trigger signal
    CyDelay(10);                    // Wait for the reading to finish
    
    return ultrasonicDistance;
}

/* [] END OF FILE */
