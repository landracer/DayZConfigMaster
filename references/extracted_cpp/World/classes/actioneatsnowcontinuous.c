// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioneatsnowcontinuous.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionEatSnowContinuousCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.DRINK_POND);
