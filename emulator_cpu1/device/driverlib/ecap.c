//###########################################################################
//
// FILE:   ecap.c
//
// TITLE: C28x ECAP driver
//
//###########################################################################
// $Copyright:  $
//###########################################################################

#include "ecap.h"

//*****************************************************************************
//
// ECAP_setEmulationMode
//
//*****************************************************************************
void ECAP_setEmulationMode(uint32_t base, ECAP_EmulationMode mode)
{
    ASSERT(ECAP_isBaseValid(base));


    //
    // Write to FREE/SOFT bit
    //
    HWREGH(base + ECAP_O_ECCTL1) =
            ((HWREGH(base + ECAP_O_ECCTL1) & (~ECAP_ECCTL1_FREE_SOFT_M)) |
             ((uint16_t)mode << ECAP_ECCTL1_FREE_SOFT_S));
}
