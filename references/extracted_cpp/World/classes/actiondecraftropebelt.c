// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondecraftropebelt.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDeCraftRopeBeltCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_DECRAFT);
