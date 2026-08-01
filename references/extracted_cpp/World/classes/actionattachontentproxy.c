// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionattachontentproxy.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionAttachOnTentProxy: ActionAttachOnProxy
{
    void ActionAttachOnTentProxy() {};
    
    override void CreateConditionComponents() 
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTCursorParent;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
