// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionemptybottlebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionEmptyBottleBaseCB : ActionContinuousBaseCB
{   
    bool m_RPCStopAlreadySent;//since stopping contains a sound tail, we need to stop it only once, this bool ensures that
    override void CreateActionComponent()
    {
        float EmptiedQuantity;
        Bottle_Base bottle = Bottle_Base.Cast(m_ActionData.m_MainItem);
        if (bottle)
            EmptiedQuantity = bottle.GetLiquidEmptyRate() * bottle.GetLiquidThroughputCoef();
        m_ActionData.m_ActionComponent = new CAContinuousEmpty(EmptiedQuantity);
