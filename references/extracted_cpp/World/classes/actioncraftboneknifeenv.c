// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioncraftboneknifeenv.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCraftBoneKnifeEnvCB : ActionContinuousBaseCB
{
    private const float TIME_TO_CRAFT_KNIFE = 5;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(TIME_TO_CRAFT_KNIFE);
