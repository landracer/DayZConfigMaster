// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/actioninteractloopbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionInteractLoopBaseCB : ActionInteractBaseCB
{
    
    bool CancelCondition()
    {
        if ( !m_ActionData )
        {
            return DefaultCancelCondition();
        }
        //SetCommand(DayZPlayerConstants.CMD_ACTIONINT_ACTIONLOOP);
        //Print(&quot;cancel condition enabled: &quot; + GetState().ToString() );
        if ( !m_Interrupted && (GetState() == STATE_LOOP_LOOP || GetState() == STATE_LOOP_LOOP2) )
        {   
            AnimatedActionBase action = AnimatedActionBase.Cast(m_ActionData.m_Action);
            action.Do(m_ActionData,m_ActionData.m_State);
        }
        return DefaultCancelCondition(); 
