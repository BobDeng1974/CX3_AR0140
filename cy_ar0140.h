/*
 * cy_ar0140.h
 *
 *  Created on: Mar 6, 2017
 *      Author: x00416012
 *      Email: xuke27@huawei.com
 */

#ifndef CY_AR0140_H_
#define CY_AR0140_H_

#include <cyu3types.h>

/* Enumeration defining Stream Format used for CyCx3_ImageSensor_Set_Format */
typedef enum CyU3PSensorStreamFormat_t
{
    SENSOR_YUV2 = 0,    /* UVC YUV2 Format*/
    SENSOR_RGB565       /* RGB565 Format  */
}CyU3PSensorStreamFormat_t;


/* Initialize Image Sensor*/
CyU3PReturnStatus_t CyCx3_ImageSensor_Init(void);




/* Put Image Sensor to Sleep/Low Power Mode */
//CyU3PReturnStatus_t CyCx3_ImageSensor_Sleep(void);

/* Wake Image Sensor from Sleep/Low Power Mode to Active Mode */
//CyU3PReturnStatus_t CyCx3_ImageSensor_Wakeup(void);



#endif /* CY_AR0140_H_ */
