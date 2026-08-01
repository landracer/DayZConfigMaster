// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionclosedoors.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCloseDoors: ActionInteractBase
{
    ref NoiseParams m_NoisePar;
    
    void ActionCloseDoors()
    {
        m_CommandUID    = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask    = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text          = &quot;#close&quot;;
