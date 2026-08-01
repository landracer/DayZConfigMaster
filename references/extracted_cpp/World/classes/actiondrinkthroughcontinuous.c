// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondrinkthroughcontinuous.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDrinkThroughContinuous: ActionDrinkPondContinuous
{
    override void CreateConditionComponents()  
    {
        m_ConditionItem     = new CCINone();
        m_ConditionTarget   = new CCTCursor(UAMaxDistances.SMALL);
