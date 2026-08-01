// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/vehicles/actionstopengineboat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionStopEngineBoat: ActionSingleUseBase
{
    void ActionStopEngineBoat()
    {
        m_CommandUID    = DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE;
        m_StanceMask    = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text          = &quot;#stop_engine&quot;;
