
/*
## Cypress CX3 configuration settings file
## ===========================
##
##  Copyright Cypress Semiconductor Corporation, 2013,
##  All Rights Reserved
##  UNPUBLISHED, LICENSED SOFTWARE.
##
##  CONFIDENTIAL AND PROPRIETARY INFORMATION
##  WHICH IS THE PROPERTY OF CYPRESS.
##
##  Use of this file is governed
##  by the license agreement included in the file
##
##     <install>/license/license.txt
##
##  where <install> is the Cypress software
##  installation root directory path.
##
## ===========================
*/

#include "cyu3mipicsi.h"


/* AR0140_YUY2_720P :  */
CyU3PMipicsiCfg_t AR0140_YUY2_720P =  
{
    CY_U3P_CSI_DF_YUV422_8_2,  /* CyU3PMipicsiDataFormat_t dataFormat */
    2,                          /* uint8_t numDataLanes */
    2,				/* uint8_t pllPrd */
    89,			/* uint16_t pllFbd */
    CY_U3P_CSI_PLL_FRS_250_500M, /* CyU3PMipicsiPllClkFrs_t pllFrs */  
    CY_U3P_CSI_PLL_CLK_DIV_4,	/* CyU3PMipicsiPllClkDiv_t csiRxClkDiv */
    CY_U3P_CSI_PLL_CLK_DIV_4,	/* CyU3PMipicsiPllClkDiv_t parClkDiv */
    0,		                /* uint16_t mClkCtl */
    CY_U3P_CSI_PLL_CLK_DIV_2,	/* CyU3PMipicsiPllClkDiv_t mClkRefDiv */
    1280,		        /* uint16_t hResolution */
    50	                        /* uint16_t fifoDelay */
};

/* [ ] */
