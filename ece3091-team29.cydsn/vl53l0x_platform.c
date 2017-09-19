/*******************************************************************************
Copyright © 2015, STMicroelectronics International N.V.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STMicroelectronics nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
NON-INFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS ARE DISCLAIMED.
IN NO EVENT SHALL STMICROELECTRONICS INTERNATIONAL N.V. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************************/

/**
 * @file VL53L0X_i2c.c
 *
 * Copyright (C) 2014 ST MicroElectronics
 *
 * provide variable word size byte/Word/dword VL6180x register access via i2c
 *
 */
#include "vl53l0x_platform.h"
#include "vl53l0x_i2c_platform.h"
#include "vl53l0x_api.h"
//#include <windows.h>
#include "project.h"

#define LOG_FUNCTION_START(fmt, ... )           _LOG_FUNCTION_START(TRACE_MODULE_PLATFORM, fmt, ##__VA_ARGS__)
#define LOG_FUNCTION_END(status, ... )          _LOG_FUNCTION_END(TRACE_MODULE_PLATFORM, status, ##__VA_ARGS__)
#define LOG_FUNCTION_END_FMT(status, fmt, ... ) _LOG_FUNCTION_END_FMT(TRACE_MODULE_PLATFORM, status, fmt, ##__VA_ARGS__)

/**
 * @def I2C_BUFFER_CONFIG
 *
 * @brief Configure Device register I2C access
 *
 * @li 0 : one GLOBAL buffer \n
 *   Use one global buffer of MAX_I2C_XFER_SIZE byte in data space \n
 *   This solution is not multi-Device compliant nor multi-thread cpu safe \n
 *   It can be the best option for small 8/16 bit MCU without stack and limited ram  (STM8s, 80C51 ...)
 *
 * @li 1 : ON_STACK/local \n
 *   Use local variable (on stack) buffer \n
 *   This solution is multi-thread with use of i2c resource lock or mutex see VL6180x_GetI2CAccess() \n
 *
 * @li 2 : User defined \n
 *    Per Device potentially dynamic allocated. Requires VL6180x_GetI2cBuffer() to be implemented.
 * @ingroup Configuration
 */
#define I2C_BUFFER_CONFIG 1
/** Maximum buffer size to be used in i2c */
#define VL53L0X_MAX_I2C_XFER_SIZE   64 /* Maximum buffer size to be used in i2c */

#if I2C_BUFFER_CONFIG == 0
    /* GLOBAL config buffer */
    uint8_t i2c_global_buffer[VL53L0X_MAX_I2C_XFER_SIZE];

    #define DECL_I2C_BUFFER
    #define VL53L0X_GetLocalBuffer(Dev, n_byte)  i2c_global_buffer

#elif I2C_BUFFER_CONFIG == 1
    /* ON STACK */
    #define DECL_I2C_BUFFER  uint8_t LocBuffer[VL53L0X_MAX_I2C_XFER_SIZE];
    #define VL53L0X_GetLocalBuffer(Dev, n_byte)  LocBuffer
#elif I2C_BUFFER_CONFIG == 2
    /* user define buffer type declare DECL_I2C_BUFFER  as access  via VL53L0X_GetLocalBuffer */
    #define DECL_I2C_BUFFER
#else
#error "invalid I2C_BUFFER_CONFIG "
#endif


#define VL53L0X_I2C_USER_VAR         /* none but could be for a flag var to get/pass to mutex interruptible  return flags and try again */
#define VL53L0X_GetI2CAccess(Dev)    /* todo mutex acquire */
#define VL53L0X_DoneI2CAcces(Dev)    /* todo mutex release */


VL53L0X_Error VL53L0X_LockSequenceAccess(VL53L0X_DEV Dev){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    return Status;
}

VL53L0X_Error VL53L0X_UnlockSequenceAccess(VL53L0X_DEV Dev){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    return Status;
}

// the ranging_sensor_comms.dll will take care of the page selection
VL53L0X_Error VL53L0X_WriteMulti(VL53L0X_DEV Dev, uint8_t index, uint8_t *pdata, uint32_t count){

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int = 0;
	uint8_t deviceAddress;

    if (count>=VL53L0X_MAX_I2C_XFER_SIZE){
        Status = VL53L0X_ERROR_INVALID_PARAMS;
    }

	deviceAddress = Dev->I2cDevAddr;

	uint8_t buf[64];

	memcpy(&buf[1], pdata, count);
	buf[0] = index;


	I2C_MasterClearStatus();
	status_int =  I2C_MasterWriteBuf(deviceAddress, buf, (uint32)(count + 1), I2C_MODE_COMPLETE_XFER);
	for (;;)
	{
		if (0u != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT))
		{
			/* Transfer complete. Check Master status to make sure that transfer
			completed without errors. */
			break;
		}
	}

	if (status_int != 0)
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

// the ranging_sensor_comms.dll will take care of the page selection
VL53L0X_Error VL53L0X_ReadMulti(VL53L0X_DEV Dev, uint8_t index, uint8_t *pdata, uint32_t count){
    VL53L0X_I2C_USER_VAR
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t last_status;
	uint8_t deviceAddress;

    if (count>=VL53L0X_MAX_I2C_XFER_SIZE){
        Status = VL53L0X_ERROR_INVALID_PARAMS;
    }

    deviceAddress = Dev->I2cDevAddr;

	last_status = I2C_MasterSendStart(deviceAddress, I2C_WRITE_XFER_MODE);
	if (I2C_MSTR_NO_ERROR == last_status)
	{
		last_status = I2C_MasterWriteByte((uint32_t)index);
		if (I2C_MSTR_NO_ERROR == last_status)
		{
			//I2C_MasterClearStatus();
			last_status = I2C_MasterReadBuf(deviceAddress, pdata, (uint32_t)count, I2C_MODE_REPEAT_START);
			for (;;)
			{
				if (0u != (I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT))
				{
					/* Transfer complete. Check Master status to make sure that transfer
					completed without errors. */
					break;
				}
			}
		}
		else
			I2C_MasterSendStop();
	}
	else
		I2C_MasterSendStop();

	if (last_status != 0)
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}


VL53L0X_Error VL53L0X_WrByte(VL53L0X_DEV Dev, uint8_t index, uint8_t data){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
	uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

	uint8_t buf[2];
	buf[0] = index;
	buf[1] = data;

	I2C_MasterClearStatus();
    //I2C_MasterSendStart(deviceAddress,0);
    //I2C_MasterWriteByte(data);
	status_int = I2C_MasterWriteBuf(deviceAddress, buf, 02u, I2C_MODE_COMPLETE_XFER);
	for (;;)
	{
		if (0u != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT))
		{
			/* Transfer complete. Check Master status to make sure that transfer
			completed without errors. */
			break;
		}
	}


	if (status_int != 0)
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

VL53L0X_Error VL53L0X_WrWord(VL53L0X_DEV Dev, uint8_t index, uint16_t data){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
	uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

	uint8_t buf[3];
	buf[0] = index;
	buf[1] = (uint8_t)((data >> 8) & 0xFF);
	buf[2] = (uint8_t)(data & 0xFF);
	I2C_MasterClearStatus();
	status_int = I2C_MasterWriteBuf(deviceAddress, buf, 03u, I2C_MODE_COMPLETE_XFER);
	for (;;)
	{
		if (0u != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT))
		{
			/* Transfer complete. Check Master status to make sure that transfer
			completed without errors. */
			break;
		}
	}

	if (status_int != 0)
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

VL53L0X_Error VL53L0X_WrDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t data){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
	uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

	uint8_t buf[5];
	buf[0] = index;
	buf[1] = (uint8_t)((data >> 24) & 0xFF);
	buf[2] = (uint8_t)((data >> 16) & 0xFF);
	buf[3] = (uint8_t)((data >> 8) & 0xFF);
	buf[4] = (uint8_t)(data & 0xFF);
	I2C_MasterClearStatus();
	status_int = I2C_MasterWriteBuf(deviceAddress, buf, 05u, I2C_MODE_COMPLETE_XFER);
	for (;;)
	{
		if (0u != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT))
		{
			/* Transfer complete. Check Master status to make sure that transfer
			completed without errors. */
			break;
		}
	}


	if (status_int != 0)
		Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

VL53L0X_Error VL53L0X_UpdateByte(VL53L0X_DEV Dev, uint8_t index, uint8_t AndData, uint8_t OrData){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t last_status;
    uint8_t deviceAddress;
    uint8_t data;

    deviceAddress = Dev->I2cDevAddr;

	last_status = I2C_MasterSendStart(deviceAddress, I2C_WRITE_XFER_MODE);
	if (I2C_MSTR_NO_ERROR == last_status)
	{
		last_status = I2C_MasterWriteByte(index);
		if (I2C_MSTR_NO_ERROR == last_status)
		{
			//I2C_MasterClearStatus();
			last_status = I2C_MasterReadBuf(deviceAddress, &data, 01u, I2C_MODE_REPEAT_START);
			for (;;)
			{
				if (0u != (I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT))
				{
					/* Transfer complete. Check Master status to make sure that transfer
					completed without errors. */
					break;
				}
			}
		}
		else
			I2C_MasterSendStop();
	}
	else
		I2C_MasterSendStop();

    if (last_status != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    if (Status == VL53L0X_ERROR_NONE) {
        data = (data & AndData) | OrData;
		
		uint8_t buf[2];
		buf[0] = index;
		buf[1] = data;

		I2C_MasterClearStatus();
		last_status = I2C_MasterWriteBuf(deviceAddress, buf, 02u, I2C_MODE_COMPLETE_XFER);
		for (;;)
		{
			if (0u != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT))
			{
				/* Transfer complete. Check Master status to make sure that transfer
				completed without errors. */
				break;
			}
		}
		

        if (last_status != 0)
            Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return Status;
}

VL53L0X_Error VL53L0X_RdByte(VL53L0X_DEV Dev, uint8_t index, uint8_t *value){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t last_status;
    uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

	uint8_t data[4];

	last_status = I2C_MasterSendStart(deviceAddress, I2C_WRITE_XFER_MODE);
	if (I2C_MSTR_NO_ERROR == last_status)
	{
		last_status = I2C_MasterWriteByte(index);
		if (I2C_MSTR_NO_ERROR == last_status)
		{
			//I2C_MasterClearStatus();
			last_status = I2C_MasterReadBuf(deviceAddress, data, 01u, I2C_MODE_REPEAT_START);
			for (;;)
			{
				if (0u != (I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT))
				{
					/* Transfer complete. Check Master status to make sure that transfer
					completed without errors. */
					break;
				}
			}
		}
		else
			I2C_MasterSendStop();
	}
	else
		I2C_MasterSendStop();

	*value = data[0]; 

    if (last_status != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

VL53L0X_Error VL53L0X_RdWord(VL53L0X_DEV Dev, uint8_t index, uint16_t *value){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t last_status;
    uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

	uint8_t data[4];
    
    last_status = I2C_MasterSendStart(deviceAddress, I2C_WRITE_XFER_MODE );
    if( I2C_MSTR_NO_ERROR == last_status )
    {
        last_status = I2C_MasterWriteByte(index);   
        if( I2C_MSTR_NO_ERROR == last_status )
        {
            //I2C_MasterClearStatus();
            last_status = I2C_MasterReadBuf(deviceAddress, data, 02u, I2C_MODE_REPEAT_START);
            for(;;)
            {
                if(0u != (I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT))
                {
                    /* Transfer complete. Check Master status to make sure that transfer
                    completed without errors. */
                break;
                }
            }
        }
        else
            I2C_MasterSendStop();
    }
    else
        I2C_MasterSendStop();
       

    *value  = ((uint16_t)data[0]&0xff)<<8;    // value high byte
	*value |= ((uint16_t)data[1]&0xff);       // value low byte

    if (last_status != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

VL53L0X_Error  VL53L0X_RdDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t *value){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t last_status;
    uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

    uint8_t data[4];
    
    last_status = I2C_MasterSendStart(deviceAddress, I2C_WRITE_XFER_MODE );
    if( I2C_MSTR_NO_ERROR == last_status )
    {
        last_status = I2C_MasterWriteByte( index);   
        if( I2C_MSTR_NO_ERROR == last_status )
        {
            //I2C_MasterClearStatus();
            last_status = I2C_MasterReadBuf(deviceAddress, data, 04u, I2C_MODE_REPEAT_START);
            for(;;)
            {
                if(0u != (I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT))
                {
                    /* Transfer complete. Check Master status to make sure that transfer
                    completed without errors. */
                break;
                }
            }
        }
        else
            I2C_MasterSendStop();
    }
    else
        I2C_MasterSendStop();
       
    *value  = ((uint32_t)data[0]&0xff)<<24;   
	*value |= ((uint32_t)data[1]&0xff)<<16;       
    *value |= ((uint32_t)data[2]&0xff)<<8; 
    *value |= ((uint32_t)data[3]&0xff); 

    if (last_status != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

#define VL53L0X_POLLINGDELAY_LOOPNB  250
VL53L0X_Error VL53L0X_PollingDelay(VL53L0X_DEV Dev){
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    LOG_FUNCTION_START("");
    /*
    const DWORD cTimeout_ms = 1;
    int hEvent = CreateEvent(0, TRUE, FALSE, 0);
    if(hEvent != NULL)
    {
        WaitForSingleObject(hEvent,cTimeout_ms);
    }
    */
    CyDelayUs(VL53L0X_POLLINGDELAY_LOOPNB);
    LOG_FUNCTION_END(status);
    return status;
}
