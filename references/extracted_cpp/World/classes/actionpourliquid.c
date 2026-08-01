// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionpourliquid.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPourLiquidCB : ActionContinuousBaseCB
{
    private const float TIME_TO_REPEAT = 0.25;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityLiquidTransfer(UAQuantityConsumed.DRAIN_LIQUID, TIME_TO_REPEAT, false);
