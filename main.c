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
#define MAIN_FILE //For global.h to only define one set of global variables

#include <stdio.h>

#include "project.h"
#include "motor.h"
#include "color.h"
#include "vl53l0x_functions.h"

#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "global.h"





CY_ISR(PID_Timer_ISR) {
    Motor_PID_Adjust(rightWheelDirection, leftWheelDirection);
    PID_Timer_ReadStatusRegister();         // Clear interrupt register
}

CY_ISR(Ultrasonic_ISR) {
    Ultrasonic_CalculateDistance();
    Ultrasonic_Timer_ReadStatusRegister();             // Clear interrupt register
}

// Interrupt routine called when RUN TASK BUTTON (PSoC) is pressed
CY_ISR(PSoC_Button_Handler) {
    PSoC_Button_ClearInterrupt();
}

// Interrupt routine called when TASK SELECTION BUTTON (external button) is pressed
CY_ISR(Ext_Button_Handler) {
    
    task = (task + 1) % 5;
    
    sprintf(string, "Selected task %d\n", task);
    UART_PutString(string);
    
    Color_LED_TurnOffAll();
    
    if(task == 4) {
        Color_LED_TurnOnAll();
    }
    else
        Color_LED_TurnOn(task);
        
    Ext_Button_ClearInterrupt(); 
}
int temp = 1;
CY_ISR(Laser_Int_Handler) {
    //temp = LaserGPIO1Pins_Read();
    //print_int_UART("%d\n",temp);
    //
    //VL53L0X_GetSensorMeasurementInterrupt(pLaserLeft, &meanFilterLeftSensor[measurementLeftNo % MEAN_FILTER_LENGTH]);
    //VL53L0X_ClearInterruptMask(pLaserLeft, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
    //VL53L0X_StartSensor(pLaserLeft, "Left");
    //VL53L0X_GetSensorMeasurementInterrupt(pLaserRight, &meanFilterRightSensor[measurementRightNo % MEAN_FILTER_LENGTH]);
    //VL53L0X_ClearInterruptMask(pLaserRight, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
    //VL53L0X_StartSensor(pLaserRight, "Right");
    
    if (!currentLaser) {
        
        VL53L0X_GetSensorMeasurementInterrupt(pLaserLeft, &meanFilterLeftSensor[measurementLeftNo % MEAN_FILTER_LENGTH]);
        VL53L0X_ClearInterruptMask(pLaserLeft, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
        VL53L0X_StartSensor(pLaserRight, "Right");
        currentLaser = right;
    }
    else {
        
        VL53L0X_GetSensorMeasurementInterrupt(pLaserRight, &meanFilterRightSensor[measurementRightNo % MEAN_FILTER_LENGTH]);
        VL53L0X_ClearInterruptMask(pLaserRight, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
        VL53L0X_StartSensor(pLaserLeft, "Left");
        currentLaser = left;
    }
    newDataReady = 1;
    LaserGPIO1Pins_ClearInterrupt();
}

int main(void)
{   
    UART_Start();       // Initializes the UART (printing to terminal)
    
    CyGlobalIntEnable;  // Enable global interrupts
    
    // Interrupt handlers
    //PID_Timer_Int_StartEx(PID_Timer_ISR);
    //Ultrasonic_Int_StartEx(Ultrasonic_ISR);
    //PSoC_Button_Int_StartEx(PSoC_Button_Handler);
    //Ext_Button_Int_StartEx(Ext_Button_Handler);
    
    
    
    // Initialization/startup code
    //Motor_Start();
    //Color_Start();
    //Ultrasonic_Timer_Start();      // Initializes the timer for the ultrasonic
    //PID_Timer_Start();
    I2C_Start();        // Initializes the I2C protocol for use with the VL53L0X laser sensors
    Laser_Initialize(VL53L0X_DEVICEMODE_SINGLE_RANGING, VL53L0X_HIGH_ACCURACY);
    
    //while(task == 0) {}
    CyDelay(3000);
    VL53L0X_ClearInterruptMask(pLaserLeft, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
    VL53L0X_ClearInterruptMask(pLaserRight, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
    //VL53L0X_StartSensor(pLaserLeft, "Left");
    VL53L0X_StartSensor(pLaserRight, "Right");
    
    Laser_Int_StartEx(Laser_Int_Handler);
    //temp = Laser_Int_GetPriority();
    //print_int_UART("Priority = %d\n",temp);
    
    

    for(;;)
    {   
        
        
        /*
        if (!temp) {
            VL53L0X_ClearInterruptMask(pLaserLeft, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
            temp = LaserGPIO1Pin_Read();
            print_int_UART("%d\n",temp);
            VL53L0X_StartSensor(pLaserLeft, "Left");
        }
        */
        if (newDataReady) {
            if (currentLaser) meanFilter(meanFilterLeftSensor,MEAN_FILTER_LENGTH,&measurementLeftNo, "Left");
            else meanFilter(meanFilterRightSensor,MEAN_FILTER_LENGTH,&measurementRightNo, "Right");
            newDataReady = 0;
        }
        
        /*
        switch(task) {
            case 1:
                run_task_1();
                break;
            
            case 2:
                run_task_2();
                break;
            
            case 3:
                run_task_3();
                //Motor_TurnRight(360);
                break;
            
            case 4:
                UART_PutString("Running task 4\n");
                while(1) {
                    int color = Color_GetColor();
    
                    Color_LED_TurnOn(color);
                    CyDelay(3000);
                    Color_LED_TurnOff(color);
                }
                break;
        }*/
        task = 0;       // Reset task option
    }
}


/* [] END OF FILE */
