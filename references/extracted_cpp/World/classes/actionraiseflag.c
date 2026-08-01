// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionraiseflag.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionManipulateFlagCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(1);
