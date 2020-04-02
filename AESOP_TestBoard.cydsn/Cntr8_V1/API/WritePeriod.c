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
#include <cydevice_trm.h>
#include <CyLib.h>

#include "`$INSTANCE_NAME`_Cntr8.h"

void `$INSTANCE_NAME`_WritePeriod(uint8 period) {
    `$INSTANCE_NAME`_Period_Reg = 255 - period;
    `$INSTANCE_NAME`_Result_Reg = 255 - period;
    }
/* [] END OF FILE */
