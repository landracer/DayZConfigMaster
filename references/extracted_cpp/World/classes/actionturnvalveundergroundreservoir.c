// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionturnvalveundergroundreservoir.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnValveUndergroundReservoirCB : ActionTurnValveCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(2);
