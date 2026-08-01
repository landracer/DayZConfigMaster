// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioneatcereal.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionEatCereal: ActionEat
{
    
    override void OnFinishProgressServer( ActionData action_data )
    {   
        super.OnFinishProgressServer(action_data);
        
        float compassChance = 0.1;
        if (Math.RandomFloatInclusive(0.0, 1.0) < compassChance)
        {
            action_data.m_Player.SpawnEntityOnGroundRaycastDispersed(&quot;OrienteeringCompass&quot;);
        }
