// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionminerock.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionMineRockCB : ActionContinuousBaseCB
{
    private const float TIME_BETWEEN_MATERIAL_DROPS = 8;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousMineRock(TIME_BETWEEN_MATERIAL_DROPS);
