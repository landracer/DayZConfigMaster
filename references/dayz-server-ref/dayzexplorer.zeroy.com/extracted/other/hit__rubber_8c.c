// ======================================================================
// File: hit__rubber_8c_source.html
// Category: other
// ======================================================================

class Hit_Rubber : EffBulletImpactBase
{
void Hit_Rubber()
{
SetEnterParticle(ParticleList.IMPACT_RUBBER_ENTER);
SetExitParticle(ParticleList.IMPACT_RUBBER_EXIT);
SetRicochetParticle(ParticleList.IMPACT_RUBBER_RICOCHET);
}
}