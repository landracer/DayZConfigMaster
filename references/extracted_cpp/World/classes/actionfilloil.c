// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/vehicles/actionfilloil.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFillOilCB : ActionContinuousBaseCB
{
    private const float TIME_TO_REPEAT = 0.05;
 
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousFillOil( UAQuantityConsumed.POUR_LIQUID, TIME_TO_REPEAT );
