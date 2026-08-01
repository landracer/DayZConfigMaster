// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/deployactions/actiondeployobject.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlaceObjectActionReciveData : ActionReciveData
{
    vector m_Position;
    vector m_Orientation;
}
 
class ActionDeployObject : ActionDeployBase
{            
    void ActionDeployObject()
    {
        m_CommandUID = 0;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
