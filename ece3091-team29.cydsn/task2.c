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
#include <math.h>

char string[100];

#define PI 3.14159265

extern float ultrasonicDistance;

void run_task_2() {
    UART_PutString("Running task 2\n");
    
    float rightDistance;
    float leftDistance;
    float difference;
    double angle;
    int i;
    
    float total = 0;
    int count = 0;
    
    difference = 0;
        
    while( fabs( difference ) >= 0 ) {
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
        
        total += difference;
        count++;
        
        sprintf(string, "Average: %f\n", total/count);
        UART_PutString(string);
        
        
        CyDelay(100);
    }
}

/* [] END OF FILE */
