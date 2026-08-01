// ======================================================================
// File: areadamageloopeddeferred_8c_source.html
// Category: other
// ======================================================================

class AreaDamageLoopedDeferred : AreaDamageLooped
{
protected float m_DeferDuration = 1.0;
override void SetDeferDuration(float time)
{
m_DeferDuration = time;
}
override protected float CalculateDamageScale(TriggerInsider insider, float deltaTime)
{
if ( m_DeferDuration > 0 && (insider.lastUpdated - insider.timeEntered) >= m_DeferDuration )
return super.CalculateDamageScale(insider, deltaTime);
return 0;
}
}