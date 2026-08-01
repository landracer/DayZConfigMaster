// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actiondisinfectplantbit.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// TO DO: Remove this script!
 
class ActionDisinfectPlantBitCB : ActionSingleUseBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CASingleUseQuantity(UAQuantityConsumed.GARDEN_DISINFECT_PLANT);
