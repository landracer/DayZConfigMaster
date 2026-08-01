// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionunfoldbandana.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUnfoldBandana : ActionUnfoldEntity
{
    void ActionUnfoldBandana ()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_Text = &quot;#bandana_unfold&quot;;
