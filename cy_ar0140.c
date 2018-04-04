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
#define AR0140_I2C_ADDRESS				(0x48)
#define AR0140_I2C_READ_ADDRESS			((AR0140_I2C_ADDRESS <<1) | 1)
#define AR0140_I2C_WRITE_ADDRESS		((AR0140_I2C_ADDRESS <<1))
#define MAX9286_CHIP_ID					(0x40)
#define MAX9286_CHIP_ID_REG				(0x1E)


/* Globals */

static CyBool_t glIsValidSensor = CyFalse;

CyU3PReturnStatus_t AR0140_SensorRead(uint8_t regAddr, uint8_t count, uint8_t *buf)
{
    CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t preamble;
    uint8_t cnt=0;

    for(cnt=0; cnt<3 ; cnt++)
    {
    	preamble.buffer[0] = AR0140_I2C_WRITE_ADDRESS; /* Slave address: write operation */
    	preamble.buffer[1] = regAddr;
    	preamble.buffer[2] = AR0140_I2C_READ_ADDRESS; /* Slave address: Read operation */
    	preamble.length = 3;
    	preamble.ctrlMask = 0x0002;
        status = CyU3PI2cReceiveBytes (&preamble, buf, count,0);
        CyU3PThreadSleep(1);
        if (status == CY_U3P_SUCCESS)
        {
        	CyU3PDebugPrint(4,"\r\nImageSensorSensorRead Successed addr=0x%x",regAddr);
            break;
        }
        else
            CyU3PDebugPrint(4,"\r\nImageSensorSensorRead Failed addr=0x%x",regAddr);
    }
    return status;
}


/* Function to verify that the image sensor is the OV5640 chip
 * Sets up flag glIsValidSensor which is to be verified before
 * any other public calls are executed*/
CyU3PReturnStatus_t AR0140_VerifyChipId()
{

    uint8_t readBuffer;

    CyU3PReturnStatus_t status = CY_U3P_SUCCESS;

    readBuffer = 0;
    status = AR0140_SensorRead(MAX9286_CHIP_ID_REG, 1, &readBuffer);
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


    CyU3PDebugPrint(4,"\r\nChip ID Read= 0x%x", readBuffer);

    return status;
}

CyU3PReturnStatus_t CyCx3_ImageSensor_Init(void)
{
	CyU3PDebugPrint(4,"\r\n ar0140 Camera Initialization");

	CyU3PReturnStatus_t status = CY_U3P_SUCCESS;

	status = AR0140_VerifyChipId();
	if (status != CY_U3P_SUCCESS)
		return status;

	return status;
}
