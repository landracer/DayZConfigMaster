// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionunrestrainself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUnrestrainSelfCB : ActionContinuousBaseCB
{
    const float DEFAULT_STRUGGLE_TIME = 2;
    
    override void CreateActionComponent()
    {
        float time = DEFAULT_STRUGGLE_TIME;
        
        if ( m_ActionData.m_MainItem.ConfigIsExisting(&quot;StruggleLength&quot;) )
        {
            time = m_ActionData.m_MainItem.ConfigGetFloat(&quot;StruggleLength&quot;);
        }
        
        #ifdef DEVELOPER
        if ( m_ActionData.m_Player.IsQuickRestrain() )
        {
            time = DEBUG_QUICK_UNRESTRAIN_TIME;
        }
        #endif
        
        m_ActionData.m_ActionComponent = new CAContinuousTime(time);
