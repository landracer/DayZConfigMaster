// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionwaterplant.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionWaterPlantCB : ActionContinuousBaseCB
{
    private const float QUANTITY_USED_PER_SEC = 150;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousWaterPlant(QUANTITY_USED_PER_SEC);
