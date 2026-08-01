// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiondetachfromtarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDetachFromTarget: ActionInteractBase
{
    
    override void CreateConditionComponents()  
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTCursor;
