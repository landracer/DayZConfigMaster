// ======================================================================
// File: hit__concrete_8c_source.html
// Category: other
// ======================================================================

class Hit_Concrete : EffBulletImpactBase
{
void Hit_Concrete()
{
SetEnterParticle(ParticleList.IMPACT_CONCRETE_ENTER);
SetExitParticle(ParticleList.IMPACT_CONCRETE_EXIT);
SetRicochetParticle(ParticleList.IMPACT_CONCRETE_RICOCHET);
m_AngledEnter = 0.50;
}
}