// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionrefueltorch.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRefuelTorch: ActionSingleUseBase
{   
    void ActionRefuelTorch()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = &quot;#STR_RefuelTorch0&quot;;
