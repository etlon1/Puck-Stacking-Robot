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

#ifndef GLOBAL_H
    #define GLOBAL_H
    
    //#include <project.h>
    #include <vl53l0x_def.h>
    #include <vl53l0x_platform.h>
    
    #ifdef MAIN_FILE
        //Laser sensor global variables
        const int MEAN_FILTER_LENGTH = 2;
        uint32_t measurementLeftNo = 0;
        uint32_t measurementRightNo = 0;
        uint16_t meanFilterLeftSensor[2];
        uint16_t meanFilterRightSensor[2];
        uint8_t newDataReady = 0;
        #define rightSensorConnected 1 //Set as 0 if the second sensor (right) is not connected, 1 if it is
        VL53L0X_Error Status = VL53L0X_ERROR_NONE;
        VL53L0X_Dev_t LaserLeft; //Left from robot's point of view
        VL53L0X_DEV pLaserLeft = &LaserLeft;
        VL53L0X_Dev_t LaserRight; //Right from robot's point of view
        VL53L0X_DEV pLaserRight = &LaserRight;
        enum uint8_t {
            left = 0,
            right = 1
        } currentLaser;
        
        char string[100];            // UART string

        int task = 0;

        int ultrasonicDistance = 0;

        int rightWheelDirection = 0;
        int leftWheelDirection = 0;
    #else
           //Laser sensor global variables
        extern const int MEAN_FILTER_LENGTH;
        extern uint32_t measurementLeftNo;
        extern uint32_t measurementRightNo;
        extern uint16_t meanFilterLeftSensor[2];
        extern uint16_t meanFilterRightSensor[2];
        extern uint8_t newDataReady;
        #define rightSensorConnected 1 //Set as 0 if the second sensor (right) is not connected, 1 if it is
        extern VL53L0X_Error Status;
        extern VL53L0X_Dev_t LaserLeft; //Left from robot's point of view
        extern VL53L0X_DEV pLaserLeft;
        extern VL53L0X_Dev_t LaserRight; //Right from robot's point of view
        extern VL53L0X_DEV pLaserRight;
        extern enum uint8_t {
            left = 0,
            right = 1
        } currentLaser;
        
        extern char string[100];            // UART string

        extern int task;

        extern int ultrasonicDistance;

        extern int rightWheelDirection;
        extern int leftWheelDirection;
        
    #endif
#endif
/* [] END OF FILE */
