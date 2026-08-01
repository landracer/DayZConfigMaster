// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/casingleuse.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CASingleUse : CASingleUseBase
{   
    override int Execute( ActionData action_data )
    {
        if ( !action_data.m_Player )
        {
            return UA_ERROR;
        }
 
        return UA_FINISHED;
