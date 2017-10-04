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

char string[100];

extern float ultrasonicDistance;

void run_task_3() {
    UART_PutString("Running task 3\n");
    
    int16 sample;
    
    while(1) {
        sample = Color_GetSample();
        
        sprintf(string, "Sample: %d\n", sample);
        UART_PutString(string);
        
        CyDelay(100);
    }
    
}

/* [] END OF FILE */
