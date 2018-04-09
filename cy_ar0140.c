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
#define 	MAX_SENSOR_NUM  			4
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

CyU3PReturnStatus_t MAX96705_SensorRead(uint8_t regAddr, uint8_t count, uint8_t *buf, uint8_t index)
{
    CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t preamble;
    uint8_t cnt=0;

    for(cnt=0; cnt<3 ; cnt++)
    {
    	preamble.buffer[0] = ((MAX96705_I2C_ADDRESS + index) << 1); /* Slave address: write operation */
    	preamble.buffer[1] = regAddr;
    	preamble.buffer[2] = (((MAX96705_I2C_ADDRESS + index)<<1) | 1); /* Slave address: Read operation */
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

CyU3PReturnStatus_t MAX96705_SensorWrite(uint8_t regAddr, uint8_t count, uint8_t buf, uint8_t index)
{
	CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
	CyU3PI2cPreamble_t preamble;
	uint8_t cnt=0;

	for(cnt=0; cnt<3 ; cnt++)
	{
		preamble.buffer[0] = ((MAX96705_I2C_ADDRESS + index) << 1); /* Slave address: write operation */
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
    status = MAX96705_SensorRead(MAX96705_CHIP_ID_REG, 1, &readBuffer, 0);
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

CyU3PReturnStatus_t Dump_Register_9286()
{
	uint8_t data = 0;
	CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
	uint8_t reg_addr[] = {
			0x15, //virtual channel
			0x0E,
			0X3F,
			0X3B,
			0X12,
			0X01,
			0x63,
            0x64,
            0x06,
            0x07,
            0x08,
            0x0C,
            0x0B,
            0x00,
            0x0A,
            0x34,
            0x36
	};
	uint8_t i;
	uint8_t readBuffer = 0 ;

	for(i=0; i < sizeof(reg_addr); i++)
	{

		status = MAX9286_SensorRead(reg_addr[i], 1, &readBuffer);
		if (status != CY_U3P_SUCCESS)
		{
			CyU3PDebugPrint(4,"\r\n MAX9286_SensorRead failed while reading address 0x%x", reg_addr[i]);
			return status;
		}
	}
}

CyU3PReturnStatus_t Dump_Register_96705(uint8_t index)
{
	uint8_t data = 0;
	CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
	uint8_t reg_addr[] = {
			0x00,
			0x07,
			0x01,
			0x0B, //control channel
			0x0C, //virtual channel
			0x47, //pixel info
			0x48,
			0X49,
			0x43,
			0x04,
			0x13,
	};
	uint8_t i;
	uint8_t readBuffer = 0 ;

	for(i=0; i < sizeof(reg_addr); i++)
	{

		status = MAX96705_SensorRead(reg_addr[i], 1, &readBuffer, index);
		if (status != CY_U3P_SUCCESS)
		{
			CyU3PDebugPrint(4,"\r\n MAX96705_SensorRead failed while reading address 0x%x", reg_addr[i]);
			return status;
		}
	}
}

CyU3PReturnStatus_t AR0140_ImageSensor_Set_Base()
{
	CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
	uint16_t g_sensor_num;
	uint8_t g_sensor_is_there;
	uint8_t reg;
	uint16_t i;
	uint8_t readBuffer = 0;
	uint8_t *cam_mask = "1000";

	status = MAX9286_SensorRead(0x49, 1, &readBuffer);
		if (status != CY_U3P_SUCCESS)
					return status;

	//Disable CSI Output
	status = MAX9286_SensorWrite(0x15, 1, 0x13);
	if (status != CY_U3P_SUCCESS)
			return status;

	//Enable PRBS test
	status = MAX9286_SensorWrite(0x0E, 1, 0x5F);
	if (status != CY_U3P_SUCCESS)
			return status;
	 /* Thread sleep : 10 ms */
		CyU3PThreadSleep(10);

	//Enable Custom Reverse Channel & First Pulse Length
	status = MAX9286_SensorWrite(0x3F, 1, 0x4F);
		if (status != CY_U3P_SUCCESS)
			return status;

	CyU3PThreadSleep(2);

	//Reverse Channel Amplitude to mid level and transition time
	status = MAX9286_SensorWrite(0x3B, 1, 0x1E);
		if (status != CY_U3P_SUCCESS)
			return status;
	CyU3PThreadSleep(2);

	//Reverse Channel Amplitude level
	status = MAX9286_SensorWrite(0x3B, 1, 0x19);
		if (status != CY_U3P_SUCCESS)
			return status;
	CyU3PThreadSleep(2);

	//Set YUV422 8 bits mode, Double Data Rate, 4 data lane
	status = MAX9286_SensorWrite(0x12, 1, 0xF3);
	//Manual Mode
	status = MAX9286_SensorWrite(0x01, 1, 0x00);
	status = MAX9286_SensorWrite(0x63, 1, 0x00);
	status = MAX9286_SensorWrite(0x64, 1, 0x00);
	status = MAX9286_SensorWrite(0x06, 1, 0x00);
	status = MAX9286_SensorWrite(0x07, 1, 0x00);
	status = MAX9286_SensorWrite(0x08, 1, 0x26);
	status = MAX9286_SensorWrite(0x0C, 1, 0x91);
	CyU3PThreadSleep(100);

	//Detect link
	g_sensor_num = 0;
	g_sensor_is_there = 0;

	for(i=0; i<4; i++)
	{
		if(cam_mask[i] == '1')
		{
			g_sensor_is_there |= (1UL<<i);
			g_sensor_num +=1;
		}
	}

	CyU3PDebugPrint(4, "\r\n max9286_mipi: snesor numer = %d.", g_sensor_num );
	CyU3PDebugPrint(4, "\r\n g_snesor_is_there = 0x%x.", g_sensor_is_there);

	//Disable PRBS test
	status = MAX9286_SensorWrite(0x0E, 1, 0x50);

	// Set link order in MIPI CSI-2 output
	reg = 0xE4; //Defaul setting
	if (g_sensor_num == 1) {
	        switch (g_sensor_is_there) {
	            case 0x8:
	                reg = 0x27;
	                break;
	            case 0x4:
	                reg = 0xC6;
	                break;
	            case 0x2:
	                reg = 0xE1;
	                break;
	            case 0x1:
	            default:
	                reg = 0xE4;
	                break;
	        }
	    } else if (g_sensor_num == 2) {
	        switch (g_sensor_is_there) {
	            case 0xC:
	                reg = 0x4E;
	                break;
	            case 0xA:
	                reg = 0x72;
	                break;
	            case 0x9:
	                reg = 0x78;
	                break;
	            case 0x6:
	                reg = 0xD2;
	                break;
	            case 0x5:
	                reg = 0xD8;
	                break;
	            case 0x3:
	            default:
	                reg = 0xE4;
	                break;
	        }
	    } else if (g_sensor_num == 3) {
	        switch (g_sensor_is_there) {
	            case 0xE:
	                reg = 0x93;
	                break;
	            case 0xD:
	                reg = 0x9C;
	                break;
	            case 0xB:
	                reg = 0xB4;
	                break;
	            case 0x7:
	            default:
	                reg = 0xE4;
	                break;
	        }
	    }

	status = MAX9286_SensorWrite(0x0B, 1, reg);

	//Enable all masked links
	reg = 0xE0 | g_sensor_is_there;
	status = MAX9286_SensorWrite(0x00, 1, reg);

	//Set up new address for each selected max96705
	reg = 0;
	for(i=1; i<= MAX_SENSOR_NUM; i++)
	{
		if( ((0x1 << (i-1)) & g_sensor_is_there) == 0)
			continue;

		//Enable Link control channel
		reg |= (0x11 << (i-1));
		status = MAX9286_SensorWrite(0x0A, 1, reg);
		CyU3PThreadSleep(2);

		status = MAX96705_SensorWrite(0x00, 1, (MAX96705_I2C_ADDRESS + i) <<1, 0 ); //(0x40 + 0) << 1   ==  0x80
		CyU3PThreadSleep(2);
		//Set MAX9271: Double Mode, PCLK latched on Rising Edge, HS/VS encoding
		status = MAX96705_SensorWrite(0x07, 1, 0x84, i );
		CyU3PThreadSleep(2);

		status = MAX96705_SensorWrite(0x01, 1, MAX9286_I2C_ADDRESS <<1, i );

		status = MAX96705_SensorWrite(0x0B, 1, (MAX96705_I2C_ADDRESS + 5) <<1, i );

		status = MAX96705_SensorWrite(0x0C, 1, (MAX96705_I2C_ADDRESS + i) <<1, i );
		CyU3PThreadSleep(1);
	}

	status = MAX9286_SensorWrite(0x0A, 1, reg );
	status = MAX9286_SensorWrite(0x0A, 1, reg );

	CyU3PThreadSleep(3);

	//Disable Local Auto I2C ACK
	status = MAX9286_SensorWrite(0x34, 1, 0x36 );

	//Enable Local Auto I2C ACK
	status = MAX9286_SensorWrite(0x34, 1, 0xB6 );

	/*add this to make up for the missing 2 lines*/
	for(i=1; i<=MAX_SENSOR_NUM; i++)
	{
		if(cam_mask[i-1]=='1')
		{
			status = MAX96705_SensorWrite(0x47, 1, 0x25, i);
			status = MAX96705_SensorWrite(0x48, 1, 0x00, i);
			status = MAX96705_SensorWrite(0x49, 1, 0x00, i);
			status = MAX96705_SensorWrite(0x43, 1, 0x25, i);
		}
	}
	/*add this to make up for the missing lines*/

	//MAX96705: Enable Serial Links and Disable Configuration Link

	for(i=1; i<=MAX_SENSOR_NUM; i++)
	{
		if(cam_mask[i-1]=='1')
		{
			status = MAX96705_SensorWrite(0x04, 1, 0x83, i);
		}
	}
	CyU3PThreadSleep(100);

	if (status != CY_U3P_SUCCESS)
		return status;

	//Read the register written
	status = MAX9286_SensorRead(0x15, 1, &readBuffer);
	if (status != CY_U3P_SUCCESS)
			return status;

	status = MAX9286_SensorRead(0x0E, 1, &readBuffer);
	if (status != CY_U3P_SUCCESS)
				return status;

	Dump_Register_9286();
	Dump_Register_96705(1);

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
