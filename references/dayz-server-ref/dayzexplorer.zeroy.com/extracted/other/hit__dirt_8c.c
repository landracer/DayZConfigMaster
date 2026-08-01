// ======================================================================
// File: hit__dirt_8c_source.html
// Category: other
// ======================================================================

class Hit_Dirt : EffBulletImpactBase
{
void Hit_Dirt()
{
SetEnterParticle(ParticleList.IMPACT_DIRT_ENTER);
SetExitParticle(ParticleList.IMPACT_DIRT_EXIT);
SetRicochetParticle(ParticleList.IMPACT_DIRT_RICOCHET);
}
}