/*******************************************************************************
* File Name: ADC_SAR_Ch1_INT.c
* Version 3.10
*
*  Description:
*    This file contains the code that operates during the ADC_SAR interrupt
*    service routine.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_SAR_Ch1.h"

extern volatile unsigned int Voltage1;

/******************************************************************************
* Custom Declarations and Variables
* - add user inlcude files, prototypes and variables between the following
*   #START and #END tags
******************************************************************************/
/* `#START ADC_SYS_VAR`  */

/* `#END`  */

#if(ADC_SAR_Ch1_IRQ_REMOVE == 0u)


    /******************************************************************************
    * Function Name: ADC_SAR_Ch1_ISR
    *******************************************************************************
    *
    * Summary:
    *  Handle Interrupt Service Routine.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    ******************************************************************************/
    CY_ISR( ADC_SAR_Ch1_ISR )
    {
        #ifdef ADC_SAR_Ch1_ISR_INTERRUPT_CALLBACK
            ADC_SAR_Ch1_ISR_InterruptCallback();
        #endif /* ADC_SAR_Ch1_ISR_INTERRUPT_CALLBACK */          

        
        /************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        *************************************************************************/
          /* `#START MAIN_ADC_ISR`  */
    
        if (ADC_SAR_Ch1_IsEndConversion(ADC_SAR_Ch1_RETURN_STATUS)) Voltage1 = (unsigned int)ADC_SAR_Ch1_GetResult16();
        else Voltage1 = 0xFFFF0000; //Error code to look for in results, upper 2 bytes should be unused
          /* `#END`  */
    }

#endif   /* End ADC_SAR_Ch1_IRQ_REMOVE */

/* [] END OF FILE */
