// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionpushcar.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPushCarData : ActionPushObjectData
{
    int m_PushDirection = -1;
    float m_HorizontalDirectionRandom   = 1.0;
    float m_VerticalDirectionRandom     = 1.0;
    
    // deprecated
    CarScript m_Car;
}
 
class CAContinuousRepeatPushCar : CAContinuousRepeatPushObject
{
    override float GetProgressWidgetMultiplier()
    {
        return 2.5;
    }
}
 
class ActionPushCarCB : ActionPushObjectCB
{   
    protected float PUSH_FORCE_IMPULSE_INCREMENT = 200.0;
    
    private ActionPushCarData m_ActionDataPushCar;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeatPushCar(UATimeSpent.PUSH_CAR);
