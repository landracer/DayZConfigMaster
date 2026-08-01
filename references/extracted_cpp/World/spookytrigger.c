// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/spookytrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SpookyTrigger extends EffectTrigger
{
 
}
 
class SpookyTrigger23 extends SpookyTrigger
{
    protected static ref SpookyTriggerEventsHandler m_EventsHandler;
 
    override void OnEnterClientEvent( TriggerInsider insider )
    {
        super.OnEnterClientEvent( insider );
        //Print(&quot;------------------------ >Entering &quot;);
        PlayerBase player = PlayerBase.Cast(insider.GetObject());
        m_EventsHandler = new SpookyTriggerEventsHandler(player);
