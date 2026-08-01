// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionrepairboatchassis.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRepairBoatChassis : ActionRepairVehiclePartBase
{
    void ActionRepairBoatChassis()
    {
        m_CallbackClass             = ActionRepairVehiclePartCB;    
        m_CommandUID                = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_StanceMask                = DayZPlayerConstants.STANCEMASK_ERECT;
        m_FullBody                  = true;
        m_LockTargetOnUse           = false;
        m_Text                      = &quot;#repair&quot;;
