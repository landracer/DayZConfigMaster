// ======================================================================
// File: hit__snow_8c_source.html
// Category: other
// ======================================================================

class Hit_Snow : EffBulletImpactBase
{
void Hit_Snow()
{
SetEnterParticle(ParticleList.IMPACT_SNOW_ENTER);
SetExitParticle(ParticleList.IMPACT_SNOW_EXIT);
SetRicochetParticle(ParticleList.IMPACT_SNOW_RICOCHET);
}
}