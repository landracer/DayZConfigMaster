// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actiontriggerremotely.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTriggerRemotely : ActionSingleUseBase
{   
    void ActionTriggerRemotely()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PRESS_TRIGGER;
        m_CommandUIDProne   = DayZPlayerConstants.CMD_ACTIONFB_PRESS_TRIGGER;
        m_Text              = &quot;#STR_Detonate&quot;;
