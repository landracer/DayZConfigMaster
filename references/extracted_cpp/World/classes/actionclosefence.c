// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionclosefence.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCloseFence: ActionInteractBase
{
    ref NoiseParams m_NoisePar;
    const string SELECTION_INTERACT = &quot;gate_interact&quot;;
    
    void ActionCloseFence()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#close&quot;;
