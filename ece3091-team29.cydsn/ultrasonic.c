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

extern float ultrasonicDistance;  // Distance based on the time of flight

void Ultrasonic_CalculateDistance() {
    int16 airTemp;          // Assume air temperature is the same as the dire
    float timeOfFlight;     // Ultrasonic wave travel time
    float soundSpeed;       // Value of sound speed based on the air temperature
    
    DieTemp_GetTemp(&airTemp);     // Get the air temperature (measured from the die temperature)
    soundSpeed = 331.3 + (0.606*airTemp);   // Calculate speed of sound based on the air temperature
    
    timeOfFlight = (float) (65535 - Ultrasonic_Timer_ReadCounter())/2000;     // Read the timer counter and calculate time
    ultrasonicDistance = soundSpeed*timeOfFlight;                    // Calculate the distance in mm
    
    //sprintf(string, "%f\n", distanceTravelled);     // Convert and store to string
    //UART_PutString(string);                         // Display the distance on the terminal
}

int Ultrasonic_EchoFree() {
    return !(Echo_FrontRight_Read() || Echo_FrontLeft_Read() || Echo_BackRight_Read() || Echo_BackLeft_Read() || Echo_SideRight_Read() || Echo_SideLeft_Read());
}

void Ultrasonic_ReadDistance(int sensorNumber) { 
    if( Ultrasonic_EchoFree() ) {
        TrigReg_Write(sensorNumber);    // Set register to 1, triggering the ultrasonic and reseting the timer
        CyDelayUs(10);                  // Trigger signal must be 10us
        TrigReg_Write(0);               // Stop ultrasonic trigger signal  
    }
}

/* [] END OF FILE */
