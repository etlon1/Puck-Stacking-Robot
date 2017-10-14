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
#ifndef _VL53L0X_FUNCTIONS_H_
#define _VL53L0X_FUNCTIONS_H_
    
#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include <malloc.h>
#include <stdio.h>

void print_pal_error(VL53L0X_Error Status);
void print_int_UART(char* string, int value);
void print_int_int_UART(char* string, int value, int value2);
void print_int_float_UART(char* string, int value, float value2);
void print_range_status(VL53L0X_RangingMeasurementData_t* pRangingMeasurementData);
float mean(uint16_t * arrayVals, int arraySize);
VL53L0X_Error VL53L0X_GetSensorMeasurementPolling(VL53L0X_DEV pMyDevice, uint32_t* measurementNo, uint16_t meanFilterSensor[], const uint8_t MEAN_FILTER_LENGTH,float* resultVar, char laserID[]);
VL53L0X_Error VL53L0X_GetSensorMeasurementInterrupt(VL53L0X_DEV pMyDevice,uint16_t* resultVar);
VL53L0X_Error WaitMeasurementDataReady(VL53L0X_DEV Dev);
VL53L0X_Error WaitStopCompleted(VL53L0X_DEV Dev);
VL53L0X_Error VL53L0X_StartSensor(VL53L0X_DEV pMyDevice, char laserID[]);
VL53L0X_Error VL53L0X_CalibrateSensor(VL53L0X_Dev_t *pMyDevice, VL53L0X_DeviceModes RangingMode, char laserID[]);
VL53L0X_Error VL53L0X_InitializeSensor(VL53L0X_DEV pMyDevice, VL53L0X_DeviceModes RangingMode, uint8_t AccuracyMode, char laserID[]);
float meanFilter(uint16_t meanFilterSensor[], const uint8_t MEAN_FILTER_LENGTH, uint32_t* measurementNo, char laserID[]);
VL53L0X_Error rangingTest(VL53L0X_Dev_t *pMyDevice);

void Laser_Initialize(uint8_t rangingeMode, uint8_t accuracyMode);
float Laser_Left_GetMeasurementPolling();
float Laser_Right_GetMeasurementPolling();

#endif
/* [] END OF FILE */
