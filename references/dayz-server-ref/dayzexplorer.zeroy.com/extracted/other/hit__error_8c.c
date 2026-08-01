// ======================================================================
// File: hit__error_8c_source.html
// Category: other
// ======================================================================

class Hit_ErrorNoMaterial : EffBulletImpactBase
{
void Hit_ErrorNoMaterial()
{
#ifdef DEVELOPER
SetSingleParticle(ParticleList.IMPACT_TEST_NO_MATERIAL_ERROR);
#else
SetSingleParticle(ParticleList.IMPACT_TEST);
#endif
}
}