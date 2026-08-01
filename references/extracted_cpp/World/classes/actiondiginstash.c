// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondiginstash.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDigInStashCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DIG_STASH);
