// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/casingleuseturnonplugged.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CASingleUseTurnOnPlugged : CASingleUse
{   
    override int Execute( ActionData action_data )
    {
        if ( !action_data.m_MainItem )
        {
            return UA_ERROR;
        }
        if ( !action_data.m_MainItem.GetCompEM().CanWork() )
        {
            return UA_FINISHED;
        }
        return UA_FINISHED;
