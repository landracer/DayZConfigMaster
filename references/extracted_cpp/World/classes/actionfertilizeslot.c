// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionfertilizeslot.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFertilizeSlotCB : ActionContinuousBaseCB
{
    private const float QUANTITY_USED_PER_SEC = 150;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousFertilizeGardenSlot(QUANTITY_USED_PER_SEC);
