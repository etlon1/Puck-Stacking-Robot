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

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_functions.h"
//#include "required_version.h"
//#include "get_nucleo_serial_comm.h"
#include <malloc.h>
#include <project.h>
#include <global.h>


//rangingMode selects single or continuous ranging
//accuracyMode == 0 for default mode or 1 for high accuracy
void Laser_Initialize(uint8_t rangingMode, uint8_t accuracyMode) {
    LaserLeft.I2cDevAddr = 0x29;
    LaserLeft.comms_speed_khz = 100; 
    LaserRight.I2cDevAddr = 0x29;
    LaserRight.comms_speed_khz = 100;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    
    uint8_t leftAddress;
    uint8_t rightAddress;
    if (rightSensorConnected == 1) {
        
        if (Status == VL53L0X_ERROR_NONE) {
            CyPins_ClearPin(RightLaserSensorXShutPin_0);
            Status = VL53L0X_SetDeviceAddress(pLaserLeft,0x52);
            LaserLeft.I2cDevAddr = 0x52;
        }
        if (Status == VL53L0X_ERROR_NONE) {
            Status = VL53L0X_InitializeSensor(pLaserLeft, rangingMode, accuracyMode, "Left");
            CyPins_SetPin(RightLaserSensorXShutPin_0);
        }
        if (Status == VL53L0X_ERROR_NONE) {
            Status = VL53L0X_RdByte(pLaserLeft, 0x008a, &leftAddress);
            print_int_UART("left Address = %d",leftAddress);
        }
        if (Status == VL53L0X_ERROR_NONE) {
            Status = VL53L0X_RdByte(pLaserRight, 0x008a, &rightAddress);
            print_int_UART("right Address = %d",rightAddress);
        }
        if (Status == VL53L0X_ERROR_NONE) {
            Status = VL53L0X_InitializeSensor(pLaserRight, rangingMode, accuracyMode, "Right");
        //VL53L0X_RdByte(pLaserRight, 0x008a, &rightAddress);
        //print_int_UART("right Address = %d",rightAddress);
        }
    }
        
        
    else {
        if (Status == VL53L0X_ERROR_NONE) {
            Status = VL53L0X_InitializeSensor(pLaserLeft, rangingMode, accuracyMode, "Left");
        }
    }
    print_pal_error(Status);
}
//////END VL53L0X Initialization//////


float Laser_Left_GetMeasurementPolling() {
    float leftSensorMeasurement;
    Status = VL53L0X_GetSensorMeasurementPolling(pLaserLeft, &measurementLeftNo, meanFilterLeftSensor, MEAN_FILTER_LENGTH, &leftSensorMeasurement, "Left");
    return leftSensorMeasurement;
}

float Laser_Right_GetMeasurementPolling() {
    float rightSensorMeasurement;  
    Status = VL53L0X_GetSensorMeasurementPolling(pLaserRight, &measurementRightNo, meanFilterRightSensor, MEAN_FILTER_LENGTH, &rightSensorMeasurement, "Right");
    return rightSensorMeasurement;
}

void print_pal_error(VL53L0X_Error Status){
    char buf[VL53L0X_MAX_STRING_LENGTH];
    char buf1[VL53L0X_MAX_STRING_LENGTH];
    VL53L0X_GetPalErrorString(Status, buf);
    sprintf(buf1, "API Status: %i : %s\n", Status,buf);
    UART_PutString(buf1);
}

void print_int_UART(char* string, int value){
    char buf[VL53L0X_MAX_STRING_LENGTH];
    sprintf(buf, string, value);
    UART_PutString(buf);
}
void print_int_int_UART(char* string, int value, int value2){
    char buf[VL53L0X_MAX_STRING_LENGTH];
    sprintf(buf, string, value, value2);
    UART_PutString(buf);
}

void print_int_float_UART(char* string, int value, float value2){
    char buf[VL53L0X_MAX_STRING_LENGTH*2];
    sprintf(buf, string, value, value2);
    UART_PutString(buf);
}

void print_int_int_string_UART(char* string, int value, int value2, char string1[]){
    char buf[VL53L0X_MAX_STRING_LENGTH*2];
    sprintf(buf, string, string1, value, value2);
    UART_PutString(buf);
}

void print_int_float_string_UART(char* string, int value, float value2, char string1[]){
    char buf[VL53L0X_MAX_STRING_LENGTH*2];
    sprintf(buf, string, string1, value, value2);
    UART_PutString(buf);
}

void print_string_UART(char* string, char string1[]){
    char buf[VL53L0X_MAX_STRING_LENGTH*2];
    sprintf(buf, string, string1);
    UART_PutString(buf);
}

void print_range_status(VL53L0X_RangingMeasurementData_t* pRangingMeasurementData){
    char buf[VL53L0X_MAX_STRING_LENGTH];
    uint8_t RangeStatus;

    /*
     * New Range Status: data is valid when pRangingMeasurementData->RangeStatus = 0
     */

    RangeStatus = pRangingMeasurementData->RangeStatus;

    VL53L0X_GetRangeStatusString(RangeStatus, buf);
    printf("Range Status: %i : %s\n", RangeStatus, buf);

}

float mean(uint16_t * arrayVals, int arraySize) {
  float meanTemp = 0;
  int i;
  for (i = 0; i < arraySize; i++) {
    meanTemp = (float) arrayVals[i] + meanTemp;
  }
  return meanTemp / arraySize;
};

VL53L0X_Error WaitMeasurementDataReady(VL53L0X_DEV Dev) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint8_t NewDatReady=0;
    uint32_t LoopNb;

    // Wait until it finished
    // use timeout to avoid deadlock
    if (Status == VL53L0X_ERROR_NONE) {
        LoopNb = 0;
        do {
            Status = VL53L0X_GetMeasurementDataReady(Dev, &NewDatReady);
            if ((NewDatReady == 0x01) || Status != VL53L0X_ERROR_NONE) {
                break;
            }
            LoopNb = LoopNb + 1;
            VL53L0X_PollingDelay(Dev);
        } while (LoopNb < VL53L0X_DEFAULT_MAX_LOOP);

        if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) {
            Status = VL53L0X_ERROR_TIME_OUT;
        }
    }

    return Status;
}

VL53L0X_Error WaitStopCompleted(VL53L0X_DEV Dev) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint32_t StopCompleted=0;
    uint32_t LoopNb;

    // Wait until it finished
    // use timeout to avoid deadlock
    if (Status == VL53L0X_ERROR_NONE) {
        LoopNb = 0;
        do {
            Status = VL53L0X_GetStopCompletedStatus(Dev, &StopCompleted);
            if ((StopCompleted == 0x00) || Status != VL53L0X_ERROR_NONE) {
                break;
            }
            LoopNb = LoopNb + 1;
            VL53L0X_PollingDelay(Dev);
        } while (LoopNb < VL53L0X_DEFAULT_MAX_LOOP);

        if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) {
            Status = VL53L0X_ERROR_TIME_OUT;
        }
	
    }

    return Status;
}
VL53L0X_Error VL53L0X_CalibrateSensor(VL53L0X_Dev_t *pMyDevice, VL53L0X_DeviceModes RangingMode, char laserID[]) {
    
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;

    if(Status == VL53L0X_ERROR_NONE)
    {   
        print_string_UART("Call of %s Sensor VL53L0X_StaticInit\n", laserID);
        Status = VL53L0X_StaticInit(pMyDevice); // Device Initialization
        // StaticInit will set interrupt by default
        print_pal_error(Status);
    }
    
    if(Status == VL53L0X_ERROR_NONE)
    {
        print_string_UART("Call of %s Sensor VL53L0X_PerformRefCalibration\n", laserID);
        Status = VL53L0X_PerformRefCalibration(pMyDevice,
        		&VhvSettings, &PhaseCal); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        print_string_UART("Call of %s Sensor VL53L0X_PerformRefSpadManagement\n", laserID);
        Status = VL53L0X_PerformRefSpadManagement(pMyDevice,
        		&refSpadCount, &isApertureSpads); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {

        print_string_UART("Call of %s Sensor VL53L0X_SetDeviceMode\n", laserID);
        Status = VL53L0X_SetDeviceMode(pMyDevice, RangingMode); // Setup in single or continuous ranging mode
        print_pal_error(Status);
    }
    return Status;
}

VL53L0X_Error VL53L0X_InitializeSensor(VL53L0X_DEV pMyDevice, VL53L0X_DeviceModes RangingMode, uint8_t AccuracyMode, char laserID[]) {
    
    print_string_UART("Call of %s Sensor VL53L0X_DataInit\n", laserID);
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    Status = VL53L0X_DataInit(pMyDevice);
    
    print_pal_error(Status);
    if (Status == VL53L0X_ERROR_NONE) print_string_UART("VL53L0X %s Sensor Initialized Successfully\n", laserID);
    else print_string_UART("VL53L0X %s Sensor Initialized Unsuccessfully\n", laserID);
    
    if (Status == VL53L0X_ERROR_NONE) {
        Status = VL53L0X_CalibrateSensor(pMyDevice, RangingMode, laserID);
    }
    if (AccuracyMode) {
        if (Status == VL53L0X_ERROR_NONE) {
            Status = VL53L0X_SetLimitCheckValue(pMyDevice,
            VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
            (FixPoint1616_t)(0.25*65536));
            }
            if (Status == VL53L0X_ERROR_NONE) {
            Status = VL53L0X_SetLimitCheckValue(pMyDevice,
            VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,
            (FixPoint1616_t)(18*65536));
            }
            if (Status == VL53L0X_ERROR_NONE) {
            Status = 
            VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice, 
            50000);
            }
    }
    
    VL53L0X_SetGpioConfig(pMyDevice, 0,VL53L0X_DEVICEMODE_SINGLE_RANGING,VL53L0X_GPIOFUNCTIONALITY_NEW_MEASURE_READY,VL53L0X_INTERRUPTPOLARITY_LOW);
    
    return Status;
}

VL53L0X_Error VL53L0X_StartSensor(VL53L0X_DEV pMyDevice, char laserID[]) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    if (Status == VL53L0X_ERROR_NONE) {
        //print_string_UART("Starting VL53L0X %s Sensor Measurement\n", laserID);
        Status = VL53L0X_StartMeasurement(pMyDevice);
        //print_pal_error(Status);
    }
    return Status;
}
VL53L0X_Error VL53L0X_GetSensorMeasurementPolling(VL53L0X_DEV pMyDevice, uint32_t* measurementNo, uint16_t meanFilterSensor[], const uint8_t MEAN_FILTER_LENGTH,float* resultVar, char laserID[]) {
    
    VL53L0X_RangingMeasurementData_t    RangingMeasurementData;
    VL53L0X_RangingMeasurementData_t   *pRangingMeasurementData    = &RangingMeasurementData;
    uint8 newDataReady = 0;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    
    Status = VL53L0X_GetMeasurementDataReady(pMyDevice, &newDataReady);
        if (newDataReady == 1u || Status != VL53L0X_ERROR_NONE) {
            if (Status == VL53L0X_ERROR_NONE)
                Status = VL53L0X_GetRangingMeasurementData(pMyDevice, pRangingMeasurementData);
            if (Status == VL53L0X_ERROR_NONE) {
                meanFilterSensor[*measurementNo % MEAN_FILTER_LENGTH] = RangingMeasurementData.RangeMilliMeter;
                //print_int_int_string_UART("%s VL53L0X Measurement %d: %d\n",*measurementNo,RangingMeasurementData.RangeMilliMeter,laserID);
                print_int_UART("%d\n",RangingMeasurementData.RangeMilliMeter);
                if (*measurementNo >= MEAN_FILTER_LENGTH - 1) {
                    *resultVar = mean(meanFilterSensor,MEAN_FILTER_LENGTH);
                    //print_int_float_string_UART("%s VL53L0X Mean Filtered Measurement %d: %.2f\n",*measurementNo - MEAN_FILTER_LENGTH + 1 ,*resultVar, laserID);  
                }
                else *resultVar = RangingMeasurementData.RangeMilliMeter;
                VL53L0X_ClearInterruptMask(pMyDevice, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
                *measurementNo = *measurementNo + 1;
            }
            else print_pal_error(Status);
        }
    return Status;
}

VL53L0X_Error VL53L0X_GetSensorMeasurementInterrupt(VL53L0X_DEV pMyDevice,uint16_t* resultVar) {
    VL53L0X_RangingMeasurementData_t    RangingMeasurementData;
    VL53L0X_Error Status = VL53L0X_GetRangingMeasurementData(pMyDevice, &RangingMeasurementData);
    *resultVar = RangingMeasurementData.RangeMilliMeter;
    return Status;
}

float meanFilter(uint16_t meanFilterSensor[], const uint8_t MEAN_FILTER_LENGTH, uint32_t* measurementNo, char laserID[]) {
    float resultVar;
    //print_int_int_string_UART("%s VL53L0X Measurement %d: %d\n",*measurementNo,meanFilterSensor[*measurementNo % MEAN_FILTER_LENGTH],laserID);
    print_int_UART("%d\n",meanFilterSensor[*measurementNo % MEAN_FILTER_LENGTH]);
    if (*measurementNo >= MEAN_FILTER_LENGTH - 1) {
        resultVar = mean(meanFilterSensor,MEAN_FILTER_LENGTH);
        //print_int_float_string_UART("%s VL53L0X Mean Filtered Measurement %d: %.2f\n",*measurementNo - MEAN_FILTER_LENGTH + 1 ,resultVar, laserID);  
    }
    else resultVar = meanFilterSensor[*measurementNo % MEAN_FILTER_LENGTH];
    
    *measurementNo = *measurementNo + 1;
    
    return resultVar;
    
}
VL53L0X_Error rangingTest(VL53L0X_Dev_t *pMyDevice)
{
    VL53L0X_RangingMeasurementData_t    RangingMeasurementData;
    VL53L0X_RangingMeasurementData_t   *pRangingMeasurementData    = &RangingMeasurementData;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;

    if(Status == VL53L0X_ERROR_NONE)
    {
        UART_PutString("Call of VL53L0X_StaticInit\n");
        Status = VL53L0X_StaticInit(pMyDevice); // Device Initialization
        // StaticInit will set interrupt by default
        print_pal_error(Status);
    }
    
    if(Status == VL53L0X_ERROR_NONE)
    {
        UART_PutString("Call of VL53L0X_PerformRefCalibration\n");
        Status = VL53L0X_PerformRefCalibration(pMyDevice,
        		&VhvSettings, &PhaseCal); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        UART_PutString("Call of VL53L0X_PerformRefSpadManagement\n");
        Status = VL53L0X_PerformRefSpadManagement(pMyDevice,
        		&refSpadCount, &isApertureSpads); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {

        UART_PutString("Call of VL53L0X_SetDeviceMode\n");
        Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING); // Setup in single ranging mode
        print_pal_error(Status);
    }
    
    if(Status == VL53L0X_ERROR_NONE)
    {
		UART_PutString("Call of VL53L0X_StartMeasurement\n");
		Status = VL53L0X_StartMeasurement(pMyDevice);
		print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        uint32_t measurement;
        uint32_t no_of_measurements = 1000000;

        uint16_t* pResults = (uint16_t*)malloc(sizeof(uint16_t) * no_of_measurements);

        for(measurement=0; measurement<no_of_measurements; measurement++)
        {

            Status = WaitMeasurementDataReady(pMyDevice);

            if(Status == VL53L0X_ERROR_NONE)
            {
                Status = VL53L0X_GetRangingMeasurementData(pMyDevice, pRangingMeasurementData);

                *(pResults + measurement) = pRangingMeasurementData->RangeMilliMeter;
                char buf[64];
                sprintf(buf,"In loop measurement %d: %d\n", (int) measurement, pRangingMeasurementData->RangeMilliMeter);
                UART_PutString(buf);

                // Clear the interrupt
                VL53L0X_ClearInterruptMask(pMyDevice, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
                VL53L0X_PollingDelay(pMyDevice);
            } else {
                break;
            }
        }

        if(Status == VL53L0X_ERROR_NONE)
        {
            for(measurement=0; measurement<no_of_measurements; measurement++)
            {
                char buf[64];
                sprintf(buf,"measurement %d: %d\n", (int) measurement, *(pResults + measurement));
                UART_PutString(buf);
            }
        }

        free(pResults);
    }

    
    if(Status == VL53L0X_ERROR_NONE)
    {
        UART_PutString("Call of VL53L0X_StopMeasurement\n");
        Status = VL53L0X_StopMeasurement(pMyDevice);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        UART_PutString("Wait Stop to be competed\n");
        Status = WaitStopCompleted(pMyDevice);
    }

    if(Status == VL53L0X_ERROR_NONE)
	Status = VL53L0X_ClearInterruptMask(pMyDevice,
		VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);

    return Status;
}
/* [] END OF FILE */
