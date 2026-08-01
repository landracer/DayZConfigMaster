// ======================================================================
// File: areadamagetriggerbase_8c_source.html
// Category: other
// ======================================================================

class AreaDamageTriggerInsider : TriggerInsider
{
float lastDamaged;
void AreaDamageTriggerInsider(Object obj)
{
}
}
class AreaDamageTriggerBase : Trigger
{
protected ref AreaDamageManager m_AreaDamageManager;
void AreaDamageTriggerBase()
{
m_AreaDamageManager = null;
}
void ~AreaDamageTriggerBase()
{
int nr = m_insiders.Count();
for (int n = 0; n < nr; ++n)
{
Object insObj = m_insiders[n].GetObject();
if ( insObj )
Leave(m_insiders[n]);
}
}
void SetAreaDamageManager( AreaDamageManager adType )
{
m_AreaDamageManager = adType;
#ifdef DEVELOPER
m_DebugAreaType = m_AreaDamageManager.GetAmmoName();
#endif
}
override protected void OnEnterBeginEvent(TriggerInsider insider)
{
super.OnEnterBeginEvent(insider);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnEnterBeginEvent(insider);
}
override protected void OnEnterServerEvent(TriggerInsider insider)
{
super.OnEnterServerEvent(insider);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnEnterServerEvent(insider);
}
override protected void OnEnterClientEvent(TriggerInsider insider)
{
super.OnEnterClientEvent(insider);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnEnterClientEvent(insider);
}
override protected void OnEnterEndEvent(TriggerInsider insider)
{
super.OnEnterEndEvent(insider);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnEnterEndEvent(insider);
}
override protected void OnStayStartBeginEvent(int nrOfInsiders)
{
super.OnStayStartBeginEvent(nrOfInsiders);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayStartBeginEvent(nrOfInsiders);
}
override protected void OnStayStartServerEvent(int nrOfInsiders)
{
super.OnStayStartServerEvent(nrOfInsiders);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayStartServerEvent(nrOfInsiders);
}
override protected void OnStayStartClientEvent(int nrOfInsiders)
{
super.OnStayStartClientEvent(nrOfInsiders);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayStartClientEvent(nrOfInsiders);
}
override protected void OnStayStartEndEvent(int nrOfInsiders)
{
super.OnStayStartEndEvent(nrOfInsiders);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayStartEndEvent(nrOfInsiders);
}
override protected void OnStayBeginEvent(TriggerInsider insider, float deltaTime)
{
super.OnStayBeginEvent(insider, deltaTime);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayBeginEvent(insider, deltaTime);
}
override protected void OnStayServerEvent(TriggerInsider insider, float deltaTime)
{
super.OnStayServerEvent(insider, deltaTime);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayServerEvent(insider, deltaTime);
}
override protected void OnStayClientEvent(TriggerInsider insider, float deltaTime)
{
super.OnStayClientEvent(insider, deltaTime);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayClientEvent(insider, deltaTime);
}
override protected void OnStayEndEvent(TriggerInsider insider, float deltaTime)
{
super.OnStayEndEvent(insider, deltaTime);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayEndEvent(insider, deltaTime);
}
override protected void OnStayFinishBeginEvent()
{
super.OnStayFinishBeginEvent();
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayFinishBeginEvent();
}
override protected void OnStayFinishServerEvent()
{
super.OnStayFinishServerEvent();
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayFinishServerEvent();
}
override protected void OnStayFinishClientEvent()
{
super.OnStayFinishClientEvent();
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayFinishClientEvent();
}
override protected void OnStayFinishEndEvent()
{
super.OnStayFinishEndEvent();
if ( m_AreaDamageManager )
m_AreaDamageManager.OnStayFinishEndEvent();
}
override protected void OnLeaveBeginEvent(TriggerInsider insider)
{
super.OnLeaveBeginEvent(insider);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnLeaveBeginEvent(insider);
}
override protected void OnLeaveServerEvent(TriggerInsider insider)
{
super.OnLeaveServerEvent(insider);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnLeaveServerEvent(insider);
}
override protected void OnLeaveClientEvent(TriggerInsider insider)
{
super.OnLeaveClientEvent(insider);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnLeaveClientEvent(insider);
}
override protected void OnLeaveEndEvent(TriggerInsider insider)
{
super.OnLeaveEndEvent(insider);
if ( m_AreaDamageManager )
m_AreaDamageManager.OnLeaveEndEvent(insider);
}
override protected bool ShouldRemoveInsider(TriggerInsider insider)
{
return !insider.GetObject().IsAlive();
}
override protected TriggerInsider CreateInsider(Object obj)
{
return new AreaDamageTriggerInsider(obj);
}
override protected void AddInsider(Object obj)
{
if ( !g_Game.IsServer() )
return;
super.AddInsider(obj);
}
override protected void RemoveInsider(TriggerInsider insider, int index = -1)
{
if ( !g_Game.IsServer() )
return;
super.RemoveInsider(insider, index);
}
override protected void UpdateInsiders(int timeout)
{
if ( !g_Game.IsServer() )
return;
super.UpdateInsiders(timeout);
}
}