// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionrestrainself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRestrainSelfCB : ActionContinuousBaseCB
{
    const float DEFAULT_RESTRAIN_TIME = 2;
    
    override void CreateActionComponent()
    {
        float time = DEFAULT_RESTRAIN_TIME;
        
        if( m_ActionData.m_MainItem.ConfigIsExisting(&quot;RestrainTime&quot;) )
        {
            time = m_ActionData.m_MainItem.ConfigGetFloat(&quot;RestrainTime&quot;);
        }
        
        if( m_ActionData.m_Player.IsQuickRestrain() )
        {
            time = DEBUG_QUICK_UNRESTRAIN_TIME;
        }
        
        m_ActionData.m_ActionComponent = new CAContinuousTime(time);
