// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionplugtargetintothis.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPlugTargetIntoThis: ActionSingleUseBase
{
    bool m_Retoggle;
    
    void ActionPlugTargetIntoThis()
    {
        m_CommandUID    = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_Retoggle      = false;
        m_Text          = &quot;#connect_together&quot;;
