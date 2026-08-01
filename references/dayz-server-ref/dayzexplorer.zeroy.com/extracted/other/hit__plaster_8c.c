// ======================================================================
// File: hit__plaster_8c_source.html
// Category: other
// ======================================================================

class Hit_Plaster : EffBulletImpactBase
{
void Hit_Plaster()
{
SetEnterParticle(ParticleList.IMPACT_PLASTER_ENTER);
SetExitParticle(ParticleList.IMPACT_PLASTER_EXIT);
SetRicochetParticle(ParticleList.IMPACT_PLASTER_RICOCHET);
}
}