// ======================================================================
// File: areadamagetriggerdirect_8c_source.html
// Category: other
// ======================================================================

class AreaDamageTriggerDirect extends AreaDamageTriggerBase
{
override void OnStayServerEvent(TriggerInsider insider, float deltaTime)
{
super.OnStayServerEvent(insider, deltaTime);
if (m_AreaDamageManager)
m_AreaDamageManager.OnEvaluateDamage(insider, deltaTime);
}
}