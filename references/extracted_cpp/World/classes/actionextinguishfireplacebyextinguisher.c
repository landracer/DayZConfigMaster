// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionextinguishfireplacebyextinguisher.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionExtinguishFireplaceByExtinguisherCB : ActionContinuousBaseCB
{
    private const float TIME_TO_REPEAT          = 0.5;
    private const float WETNESS_GAIN_MULTIPLIER = 0.5;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityExtinguish(UAQuantityConsumed.FIREPLACE_EXTINGUISHER, TIME_TO_REPEAT, WETNESS_GAIN_MULTIPLIER);
        CAContinuousQuantityExtinguish.Cast(m_ActionData.m_ActionComponent).MultiplyQuantityUsed(m_ActionData.m_MainItem.GetLiquidThroughputCoef());
