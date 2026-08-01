// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionloadmagazine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionLoadMagazineCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.MAG_LOAD);
