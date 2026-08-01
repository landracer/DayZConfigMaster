// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionunpackgift.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUnpackGiftCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.UNPACK);
