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
#if !defined(`$INSTANCE_NAME`_Cntr8_H)
#define `$INSTANCE_NAME`_Cntr8_H
    
#include "`$INSTANCE_NAME`_defs.h"
#define `$INSTANCE_NAME`_Period_Reg  `$INSTANCE_NAME`_Datapath_1_D0_REG
#define `$INSTANCE_NAME`_Result_Reg  `$INSTANCE_NAME`_Datapath_1_A0_REG
    
void `$INSTANCE_NAME`_WritePeriod(uint8 period);
uint8 `$INSTANCE_NAME`_ReadCount();
    
#endif
/* [] END OF FILE */
