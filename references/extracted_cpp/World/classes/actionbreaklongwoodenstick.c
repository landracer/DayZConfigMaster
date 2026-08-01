// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionbreaklongwoodenstick.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionBreakLongWoodenStickCB : ActionContinuousBaseCB
{
    private const float TIME_TO_BREAK_STICKS = 5.0;
    
    override void CreateActionComponent()
    {
        //float adjusted_time = m_ActionData.m_Player.GetSoftSkillsManager().AdjustCraftingTime(TIME_TO_BREAK_STICKS,UASoftSkillsWeight.ROUGH_HIGH);    
        m_ActionData.m_ActionComponent = new CAContinuousTime(TIME_TO_BREAK_STICKS);
