// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actionburnsewself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionBurnSewSelfCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SEW_WOUNDS);
