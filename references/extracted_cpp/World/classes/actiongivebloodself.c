// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actiongivebloodself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionGiveBloodData : ActionData
{
    int m_ItemBloodType;
    float m_BloodAmount;
    int m_Agents;
}
 
class ActionGiveBloodSelfCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityBloodTransfer(UAQuantityConsumed.BLOOD, UATimeSpent.BLOOD);
