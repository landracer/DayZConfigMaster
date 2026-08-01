// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioncraftboltsfeather.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCraftBoltsFeatherCB : ActionContinuousBaseCB
{
    private const float TIME_TO_CRAFT = 3.0;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(TIME_TO_CRAFT);
