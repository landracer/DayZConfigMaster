// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondecraftwitchhoodcoif.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDeCraftWitchHoodCoifCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_DECRAFT);
