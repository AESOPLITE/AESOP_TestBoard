/* ========================================
 *
 * Copyright UCSC, 2020
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * ========================================
*/
#if !defined(`$INSTANCE_NAME`_ACD_CTRL_H)
#define `$INSTANCE_NAME`_ACD_CTRL_H
#include <cydevice_trm.h>
#include <CyLib.h>

#define `$INSTANCE_NAME`_Datapath_1_A0_PTR 	 ((reg8 *) `$INSTANCE_NAME`_Datapath_1_u0__A0_REG)
#define `$INSTANCE_NAME`_Datapath_1_A0_REG 	 (* `$INSTANCE_NAME`_Datapath_1_A0_PTR)
#define `$INSTANCE_NAME`_Datapath_1_D0_PTR 	 ((reg8 *) `$INSTANCE_NAME`_Datapath_1_u0__D0_REG)
#define `$INSTANCE_NAME`_Datapath_1_D0_REG 	 (* `$INSTANCE_NAME`_Datapath_1_D0_PTR)    
#define `$INSTANCE_NAME`_Period_Reg  `$INSTANCE_NAME`_Datapath_1_D0_REG
#define `$INSTANCE_NAME`_Result_Reg  `$INSTANCE_NAME`_Datapath_1_A0_REG
    
void `$INSTANCE_NAME`_WritePeriod(uint8 period);
    
#endif
/* [] END OF FILE */
