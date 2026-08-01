// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioncraftboneknife.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCraftBoneKnifeCB : ActionContinuousBaseCB
{   
    override void CreateActionComponent()
    {   
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.DEFAULT_CRAFT);
