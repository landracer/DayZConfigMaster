// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/actioncontinuousbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionContinuousBaseCB : ActionBaseCB
{   
    bool m_inLoop = false;
    bool m_callLoopEnd = false;
        
    bool CancelCondition()
    {
        if ( !m_ActionData )
        {
            return DefaultCancelCondition();
        }
        if ((GetState() == STATE_LOOP_LOOP || GetState() == STATE_LOOP_LOOP2 || m_inLoop) )
        {
            ActionContinuousBase actionS = ActionContinuousBase.Cast(m_ActionData.m_Action);    
            if ( m_ActionData.m_State == UA_INITIALIZE )
            {
                actionS.OnStartAnimationLoop( m_ActionData );
                m_ActionData.m_State = UA_PROCESSING;
                m_callLoopEnd = true;
            }
            
            if (!m_Interrupted)
                actionS.Do(m_ActionData, m_ActionData.m_State);
        }
        else if (m_callLoopEnd == true)
        {
            ActionContinuousBase actionE = ActionContinuousBase.Cast(m_ActionData.m_Action);
            actionE.OnEndAnimationLoop( m_ActionData );
            m_callLoopEnd = false;
        }
        return DefaultCancelCondition();
