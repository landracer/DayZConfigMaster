// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/vehicles/actionfillfuel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFillFuelCB : ActionContinuousBaseCB
{
    private const float TIME_TO_REPEAT = 0.5;
 
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousFillFuel( UAQuantityConsumed.FUEL, TIME_TO_REPEAT );
