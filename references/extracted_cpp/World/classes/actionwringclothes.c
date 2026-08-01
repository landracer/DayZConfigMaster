// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionwringclothes.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionWringClothesCB : ActionContinuousBaseCB
{
    protected const float QUANTITY_WRINGLED_PER_SECOND = 0.02;
    
    override void CreateActionComponent()
    {
        //m_ActionData.m_ActionComponent = new CAContinuousWringClothes(QUANTITY_WRINGLED_PER_SECOND, UATimeSpent.WASH_HANDS);
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.WRING);
