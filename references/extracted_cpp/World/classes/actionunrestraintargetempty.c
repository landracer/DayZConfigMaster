// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionunrestraintargetempty.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUnrestrainTargetHandsCB : ActionContinuousBaseCB
{
    const float DEFAULT_STRUGGLE_TIME = 2;
    const float UNRESTRAIN_STRUGGLE_MULTIPLIER = 0.75;//multiplier for the struggle time normally used for the player unrestraining themselves, this allows unrestraining to be bound to the &#39;StruggleLength&#39; config param
    
    override void CreateActionComponent()
    {
        float time = DEFAULT_STRUGGLE_TIME;
        PlayerBase targetPlayer = PlayerBase.Cast(m_ActionData.m_Target.GetObject());
        EntityAI item_in_hands = targetPlayer.GetItemInHands();
        
        if ( item_in_hands.ConfigIsExisting(&quot;StruggleLength&quot;) )
        {
            time = item_in_hands.ConfigGetFloat(&quot;StruggleLength&quot;) * UNRESTRAIN_STRUGGLE_MULTIPLIER;
        }
        #ifdef DEVELOPER
        if ( m_ActionData.m_Player.IsQuickRestrain() )
        {
            time = DEBUG_QUICK_UNRESTRAIN_TIME;
        }
        #endif
        
        m_ActionData.m_ActionComponent = new CAContinuousTime(time);
