// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/vehicles/actionfillbrakes.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFillBrakesCB : ActionContinuousBaseCB
{
    private const float TIME_TO_REPEAT = 0.05;
 
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousFillBrakes( UAQuantityConsumed.POUR_LIQUID, TIME_TO_REPEAT );
