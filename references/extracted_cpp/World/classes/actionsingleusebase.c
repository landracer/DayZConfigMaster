// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/actionsingleusebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionSingleUseBaseCB : ActionBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CASingleUse;
