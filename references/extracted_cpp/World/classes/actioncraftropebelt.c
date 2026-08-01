// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioncraftropebelt.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCraftRopeBeltCB : ActionContinuousBaseCB
{   
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(3);
