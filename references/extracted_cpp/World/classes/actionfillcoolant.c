// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/vehicles/actionfillcoolant.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFillCoolantCB : ActionContinuousBaseCB
{
    private const float TIME_TO_REPEAT = 0.075;
 
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousFillCoolant( UAQuantityConsumed.FUEL, TIME_TO_REPEAT );
