// ======================================================================
// File: triggerevents_8c_source.html
// Category: other
// ======================================================================

class TriggerEvents : ScriptedEntity
{
protected ref ScriptInvoker m_OnEnterInvoker;
protected ref ScriptInvoker m_OnLeaveInvoker;
ScriptInvoker GetOnEnterInvoker()
{
if ( !m_OnEnterInvoker )
m_OnEnterInvoker = new ScriptInvoker;
return m_OnEnterInvoker;
}
ScriptInvoker GetOnLeaveInvoker()
{
if ( !m_OnLeaveInvoker )
m_OnLeaveInvoker = new ScriptInvoker;
return m_OnLeaveInvoker;
}
protected void Enter(TriggerInsider insider)
{
#ifdef TRIGGER_DEBUG_BASIC
Debug.TriggerLog(GetDebugName(insider.GetObject()), "TriggerEvents", "", "Enter", GetDebugName(this));
#endif
OnEnterBeginEvent(insider);
if (m_OnEnterInvoker)
m_OnEnterInvoker.Invoke(insider);
if ( g_Game.IsServer() )
OnEnterServerEvent(insider);
else
OnEnterClientEvent(insider);
OnEnterEndEvent(insider);
}
protected void OnEnterBeginEvent(TriggerInsider insider) {}
protected void OnEnterServerEvent(TriggerInsider insider) {}
protected void OnEnterClientEvent(TriggerInsider insider) {}
protected void OnEnterEndEvent(TriggerInsider insider) {}
protected void StayStart(int nrOfInsiders)
{
#ifdef TRIGGER_DEBUG_SPAM
Debug.TriggerLog(nrOfInsiders.ToString(), "TriggerEvents", "", "StayStart", GetDebugName(this));
#endif
OnStayStartBeginEvent(nrOfInsiders);
if ( g_Game.IsServer() )
OnStayStartServerEvent(nrOfInsiders);
else
OnStayStartClientEvent(nrOfInsiders);
OnStayStartEndEvent(nrOfInsiders);
}
protected void OnStayStartBeginEvent(int nrOfInsiders) {}
protected void OnStayStartServerEvent(int nrOfInsiders) {}
protected void OnStayStartClientEvent(int nrOfInsiders) {}
protected void OnStayStartEndEvent(int nrOfInsiders) {}
protected void Stay(TriggerInsider insider, float deltaTime)
{
#ifdef TRIGGER_DEBUG_SPAM
Debug.TriggerLog(GetDebugName(insider.GetObject()), "TriggerEvents", "", "Stay", GetDebugName(this));
#endif
OnStayBeginEvent(insider, deltaTime);
if ( g_Game.IsServer() )
OnStayServerEvent(insider, deltaTime);
else
OnStayClientEvent(insider, deltaTime);
OnStayEndEvent(insider, deltaTime);
}
protected void OnStayBeginEvent(TriggerInsider insider, float deltaTime) {}
protected void OnStayServerEvent(TriggerInsider insider, float deltaTime) {}
protected void OnStayClientEvent(TriggerInsider insider, float deltaTime) {}
protected void OnStayEndEvent(TriggerInsider insider, float deltaTime) {}
protected void StayFinish()
{
#ifdef TRIGGER_DEBUG_SPAM
Debug.TriggerLog("", "TriggerEvents", "", "StayFinish", GetDebugName(this));
#endif
OnStayFinishBeginEvent();
if ( g_Game.IsServer() )
OnStayFinishServerEvent();
else
OnStayFinishClientEvent();
OnStayFinishEndEvent();
}
protected void OnStayFinishBeginEvent() {}
protected void OnStayFinishServerEvent() {}
protected void OnStayFinishClientEvent() {}
protected void OnStayFinishEndEvent() {}
protected void Leave(TriggerInsider insider)
{
#ifdef TRIGGER_DEBUG_BASIC
Debug.TriggerLog(GetDebugName(insider.GetObject()), "TriggerEvents", "", "Leave", GetDebugName(this));
#endif
OnLeaveBeginEvent(insider);
if (m_OnLeaveInvoker)
m_OnLeaveInvoker.Invoke(insider);
if ( g_Game && g_Game.IsServer() )
OnLeaveServerEvent(insider);
else
OnLeaveClientEvent(insider);
OnLeaveEndEvent(insider);
}
protected void OnLeaveBeginEvent(TriggerInsider insider) {}
protected void OnLeaveServerEvent(TriggerInsider insider) {}
protected void OnLeaveClientEvent(TriggerInsider insider) {}
protected void OnLeaveEndEvent(TriggerInsider insider) {}
}