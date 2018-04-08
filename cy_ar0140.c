/*
 * cy_ar0140.c
 *
*  Created on: Mar 6, 2017
 *      Author: x00416012
 *      Email: xuke27@huawei.com
 */


#include "cy_ar0140.h"
#include <cyu3error.h>
#include <cyu3i2c.h>
#include <cyu3utils.h>

/* AR0140 Misc Macros*/
#define MAX9286_I2C_ADDRESS				(0x48)
#define MAX9286_I2C_READ_ADDRESS		((MAX9286_I2C_ADDRESS <<1) | 1)
#define MAX9286_I2C_WRITE_ADDRESS		((MAX9286_I2C_ADDRESS <<1))
#define MAX9286_CHIP_ID					(0x40)
#define MAX9286_CHIP_ID_REG				(0x1E)

#define MAX96705_I2C_ADDRESS			(0x40)
#define MAX96705_I2C_READ_ADDRESS		((MAX96705_I2C_ADDRESS <<1) | 1)
#define MAX96705_I2C_WRITE_ADDRESS		((MAX96705_I2C_ADDRESS <<1))
#define MAX96705_CHIP_ID				(0x41)
#define MAX96705_CHIP_ID_REG			(0x1E)

/* Globals */

static CyBool_t glIsValidSensor = CyFalse;

CyU3PReturnStatus_t MAX9286_SensorRead(uint8_t regAddr, uint8_t count, uint8_t *buf)
{
    CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t preamble;
    uint8_t cnt=0;

    for(cnt=0; cnt<3 ; cnt++)
    {
    	preamble.buffer[0] = MAX9286_I2C_WRITE_ADDRESS; /* Slave address: write operation */
    	preamble.buffer[1] = regAddr;
    	preamble.buffer[2] = MAX9286_I2C_READ_ADDRESS; /* Slave address: Read operation */
    	preamble.length = 3;
    	preamble.ctrlMask = 0x0002;
        status = CyU3PI2cReceiveBytes (&preamble, buf, count,0);
        CyU3PThreadSleep(1);
        if (status == CY_U3P_SUCCESS)
        {
        	CyU3PDebugPrint(4,"\r\n MAX9286_SensorRead Succeed addr=0x%x with reg=0x%x",regAddr, *buf);
            break;
        }
        else
            CyU3PDebugPrint(4,"\r\n MAX9286_SensorRead Failed addr=0x%x with reg=0x%x",regAddr, *buf);
    }
    return status;
}

CyU3PReturnStatus_t MAX9286_SensorWrite(uint8_t regAddr, uint8_t count, uint8_t buf)
{
	CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
	    CyU3PI2cPreamble_t preamble;
	    uint8_t cnt=0;

	    for(cnt=0; cnt<3 ; cnt++)
	    {
	    	preamble.buffer[0] = MAX9286_I2C_WRITE_ADDRESS; /* Slave address: write operation */
	    	preamble.buffer[1] = regAddr;
	    	preamble.length = 2;
	    	preamble.ctrlMask = 0x0000;
	        status = CyU3PI2cTransmitBytes (&preamble, &buf, count,0);
	        CyU3PThreadSleep(1);
	        if (status == CY_U3P_SUCCESS)
	        {
	        	CyU3PDebugPrint(4,"\r\n MAX9286_SensorWrite Succeed addr=0x%x with reg=0x%x",regAddr, buf);
	            break;
	        }
	        else
	            CyU3PDebugPrint(4,"\r\n MAX9286_SensorWrite Failed addr=0x%x with reg=0x%x",regAddr, buf);
	    }
	    return status;
}

CyU3PReturnStatus_t MAX96705_SensorRead(uint8_t regAddr, uint8_t count, uint8_t *buf)
{
    CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t preamble;
    uint8_t cnt=0;

    for(cnt=0; cnt<3 ; cnt++)
    {
    	preamble.buffer[0] = MAX96705_I2C_WRITE_ADDRESS; /* Slave address: write operation */
    	preamble.buffer[1] = regAddr;
    	preamble.buffer[2] = MAX96705_I2C_READ_ADDRESS; /* Slave address: Read operation */
    	preamble.length = 3;
    	preamble.ctrlMask = 0x0002;
        status = CyU3PI2cReceiveBytes (&preamble, buf, count,0);
        CyU3PThreadSleep(1);
        if (status == CY_U3P_SUCCESS)
        {
        	CyU3PDebugPrint(4,"\r\n MAX96705_SensorRead Succeed addr=0x%x with reg=0x%x",regAddr, *buf);
            break;
        }
        else
            CyU3PDebugPrint(4,"\r\n MAX96705_SensorRead Failed addr=0x%x with reg=0x%x",regAddr, *buf);
    }
    return status;
}

CyU3PReturnStatus_t MAX96705_SensorWrite(uint8_t regAddr, uint8_t count, uint8_t buf)
{
	CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
	CyU3PI2cPreamble_t preamble;
	uint8_t cnt=0;

	for(cnt=0; cnt<3 ; cnt++)
	{
		preamble.buffer[0] = MAX96705_I2C_WRITE_ADDRESS; /* Slave address: write operation */
		preamble.buffer[1] = regAddr;
		preamble.length = 2;
		preamble.ctrlMask = 0x0000;
		status = CyU3PI2cTransmitBytes (&preamble, &buf, count,0);
		CyU3PThreadSleep(1);
		if (status == CY_U3P_SUCCESS)
		{
			CyU3PDebugPrint(4,"\r\n MAX96705_SensorWrite Succeed addr=0x%x with reg=0x%x",regAddr, buf);
			break;
		}
		else
			CyU3PDebugPrint(4,"\r\n MAX96705_SensorWrite Failed addr=0x%x with reg=0x%x",regAddr, buf);
	}
	return status;
}
/* Function to verify that the image sensor is the MAX9286 chip
 * Sets up flag glIsValidSensor which is to be verified before
 * any other public calls are executed*/
CyU3PReturnStatus_t MAX9286_VerifyChipId()
{

    uint8_t readBuffer;

    CyU3PReturnStatus_t status = CY_U3P_SUCCESS;

    readBuffer = 0;
    status = MAX9286_SensorRead(MAX9286_CHIP_ID_REG, 1, &readBuffer);
    if (status != CY_U3P_SUCCESS)
        return status;

    if (MAX9286_CHIP_ID != readBuffer)
    {
        status = CY_U3P_ERROR_BAD_ARGUMENT;
    }
    else
    {
        glIsValidSensor = CyTrue;
    }


    CyU3PDebugPrint(4,"\r\n Chip ID Read= 0x%x", readBuffer);

    return status;
}

/* Function to verify that the image sensor is the MAX9286 chip
 * Sets up flag glIsValidSensor which is to be verified before
 * any other public calls are executed*/
CyU3PReturnStatus_t MAX96705_VerifyChipId()
{

    uint8_t readBuffer;

    CyU3PReturnStatus_t status = CY_U3P_SUCCESS;

    readBuffer = 0;
    status = MAX96705_SensorRead(MAX96705_CHIP_ID_REG, 1, &readBuffer);
    if (status != CY_U3P_SUCCESS)
        return status;

    if (MAX96705_CHIP_ID != readBuffer)
    {
        status = CY_U3P_ERROR_BAD_ARGUMENT;
    }
    else
    {
        glIsValidSensor = CyTrue;
    }


    CyU3PDebugPrint(4,"\r\n Chip ID Read= 0x%x", readBuffer);

    return status;
}

CyU3PReturnStatus_t AR0140_ImageSensor_Set_Base()
{
	CyU3PReturnStatus_t status = CY_U3P_SUCCESS;

	uint8_t readBuffer = 0;
	//Disable CSI Output
	status = MAX9286_SensorWrite(0x15, 1, 0x13);
	if (status != CY_U3P_SUCCESS)
			return status;
	//Enable PRBS test
	status = MAX9286_SensorWrite(0x0E, 1, 0x5F);
	if (status != CY_U3P_SUCCESS)
			return status;
	//Read the register written
	status = MAX9286_SensorRead(0x15, 1, &readBuffer);
	if (status != CY_U3P_SUCCESS)
			return status;

	status = MAX9286_SensorRead(0x0E, 1, &readBuffer);
	if (status != CY_U3P_SUCCESS)
				return status;

	return status;
}

CyU3PReturnStatus_t CyCx3_ImageSensor_Init(void)
{
	CyU3PDebugPrint(4,"\r\n ar0140 Camera Initialization");

	CyU3PReturnStatus_t status = CY_U3P_SUCCESS;

	status = MAX9286_VerifyChipId();
	if (status != CY_U3P_SUCCESS)
		return status;

	status = MAX96705_VerifyChipId();
	if (status != CY_U3P_SUCCESS)
			return status;

	status = AR0140_ImageSensor_Set_Base();
	if (status != CY_U3P_SUCCESS)
				return status;

	return status;
}
