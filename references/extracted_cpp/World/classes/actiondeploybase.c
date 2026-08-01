// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/deployactions/actiondeploybase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlaceObjectActionData : ActionData
{
    vector m_Position;
    vector m_Orientation;
    bool m_AlreadyPlaced;
}
 
class ActiondeployObjectCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(m_ActionData.m_MainItem.GetDeployTime());
