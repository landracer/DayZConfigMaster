// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionwatergardenslot.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionWaterGardenSlotCB : ActionContinuousBaseCB
{
    private const float QUANTITY_USED_PER_SEC = 150;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousWaterSlot(QUANTITY_USED_PER_SEC);
