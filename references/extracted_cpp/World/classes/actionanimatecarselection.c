// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/vehicles/actionanimatecarselection.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionAnimateCarSelection: ActionInteractBase
{
    string m_AnimSource = &quot;&quot;;
 
    void ActionAnimateCarSelection()
    {
        m_CommandUID    = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask    = DayZPlayerConstants.STANCEMASK_ALL;
        m_Text = &quot;#manipulate&quot;;
