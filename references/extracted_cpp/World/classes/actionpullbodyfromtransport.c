// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionpullbodyfromtransport.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPullBodyFromTransport: ActionInteractBase
{
    void ActionPullBodyFromTransport()
    {
        m_StanceMask    = DayZPlayerConstants.STANCEMASK_ALL;
        m_Text          = &quot;#pull_out_body&quot;;
