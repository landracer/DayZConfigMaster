// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioncraftstoneknifeenv.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCraftStoneKnifeEnvCB : ActionContinuousBaseCB
{
    private const float TIME_TO_CRAFT_KNIFE = 5;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(TIME_TO_CRAFT_KNIFE);
