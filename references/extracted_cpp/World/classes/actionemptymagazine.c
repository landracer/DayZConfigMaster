// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionemptymagazine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EmptyMagazineActionData : ActionData
{
    Magazine m_magazinePile;
}
 
class ActionEmptyMagazineCB : ActionContinuousBaseCB
{   
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.MAG_EMPTY);
