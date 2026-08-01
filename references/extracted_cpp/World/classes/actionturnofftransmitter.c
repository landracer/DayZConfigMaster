// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionturnofftransmitter.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnOffTransmitterCB : ActionSingleUseBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CASingleUse();
