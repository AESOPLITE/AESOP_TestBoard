/* ========================================
 *
 * Copyright UCSC, 2020
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * ========================================
*/

#include "`$INSTANCE_NAME`_ACD_CTRL.h"

void `$INSTANCE_NAME`_WritePeriod(uint8 period) {
    `$INSTANCE_NAME`_Period_Reg = period;
    `$INSTANCE_NAME`_Result_Reg = period;
    }
/* [] END OF FILE */
