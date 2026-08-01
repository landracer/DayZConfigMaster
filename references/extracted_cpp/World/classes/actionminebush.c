// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionminebush.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionMineBushCB : ActionContinuousBaseCB
{
    protected const float TIME_BETWEEN_MATERIAL_DROPS_DEFAULT = 3;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousMineWood(TIME_BETWEEN_MATERIAL_DROPS_DEFAULT);
