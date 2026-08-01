// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actiongivesalineself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionGiveSalineSelfCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SALINE);
