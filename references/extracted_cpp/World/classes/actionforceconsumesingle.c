// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionforceconsumesingle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionForceConsumeSingleCB : ActionSingleUseBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CASingleUseQuantityEdible(UAQuantityConsumed.DEFAULT);
