// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actiontakeabite.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTakeABiteCB : ActionSingleUseBaseCB
{
    private const float QUANTITY_USED_PER_SEC2 = 50;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CASingleUseQuantityEdible(QUANTITY_USED_PER_SEC2);
