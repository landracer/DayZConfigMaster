// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioncraftimprovisedfeetcover.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCraftImprovisedFeetCoverCB : ActionContinuousBaseCB
{
    private const float TIME_TO_CRAFT_CLOTHES = 5.0;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(TIME_TO_CRAFT_CLOTHES);
