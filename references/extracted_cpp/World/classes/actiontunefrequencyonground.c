// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiontunefrequencyonground.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTuneFrequencyOnGround : ActionInteractBase
{
    string m_RadioFreq;
 
    void ActionTuneFrequencyOnGround()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_RadioFreq         = string.Empty;
        m_Text = &quot;#tune_frequency&quot;;
