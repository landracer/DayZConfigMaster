// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionignitefireplacebyair.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionIgniteFireplaceByAirCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.FIREPLACE_IGNITE );
