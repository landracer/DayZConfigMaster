// ======================================================================
// File: casingleuseturnonplugged_8c_source.html
// Category: other
// ======================================================================

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
}
};