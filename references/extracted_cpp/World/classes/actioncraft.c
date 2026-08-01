// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioncraft.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCraftCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousCraft(0); //default value can be set in recipes
