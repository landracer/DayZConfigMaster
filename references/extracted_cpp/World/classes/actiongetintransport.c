// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiongetintransport.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionGetInTransport: ActionBase
{
    void ActionGetInTransport()
    {
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#get_in_vehicle&quot;;
