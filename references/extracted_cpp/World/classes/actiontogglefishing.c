// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actiontogglefishing.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*class ActionToggleFishing: ActionSingleUseBase
{
    void ActionToggleFishing()
    {
        m_Sound = &quot;CastingRod&quot;;
    }
    
    override void CreateConditionComponents()  
    {   
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTNone;
    }
 
    override bool HasTarget()
    {
        return false;
    }
 
    override string GetText()
    {
        return &quot;TODO&quot;;
    }
 
    override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
    {
        return true;
        
        vector pos_cursor = target.GetCursorHitPos();
        float distance = Math.AbsInt(vector.Distance(pos_cursor,player.GetPosition()));
        if ( distance <= 2 )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
 
    override void OnExecuteServer( ActionData action_data )
    {
        FishingRod_Base nitem = FishingRod_Base.Cast( action_data.m_MainItem );
        if( action_data.m_MainItem.GetAnimationPhase(&quot;OpenRod&quot;) )
        {
            //nitem.DeactivateFishing();
            action_data.m_MainItem.SetAnimationPhase(&quot;CloseRod&quot;,1);
            action_data.m_MainItem.SetAnimationPhase(&quot;OpenRod&quot;,0);
        }
        else
        {
            //nitem.ActivateFishing();
            action_data.m_MainItem.SetAnimationPhase(&quot;CloseRod&quot;,0);
            action_data.m_MainItem.SetAnimationPhase(&quot;OpenRod&quot;,1);
        }
    }
