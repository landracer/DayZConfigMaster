// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionunrestraintarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUnrestrainTargetCB : ActionContinuousBaseCB
{
    const float DEFAULT_UNRESTRAIN_TIME = 2;
    
    override void CreateActionComponent()
    {
        float time = ObtainUnrestrainTime();
        
        if( time <=0 )
        {
            time = DEFAULT_UNRESTRAIN_TIME;
        }
        
        if( m_ActionData.m_Player.IsQuickRestrain() )
        {
            time = DEBUG_QUICK_UNRESTRAIN_TIME;
        }
 
        m_ActionData.m_ActionComponent = new CAContinuousTime(time);
