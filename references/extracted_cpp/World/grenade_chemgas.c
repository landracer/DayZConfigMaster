// Source: F:/Games/Dayz/scripts/4_world/entities/grenade_base/grenade_chemgas.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Grenade_ChemGas : Grenade_Base
{
    protected bool                  m_Exploded;
    protected ParticleSource        m_ParticleExploded;
    protected EffectSound           m_ExplosionSound;
    
    
    void Grenade_ChemGas()
    {
        SetParticleExplosion(ParticleList.RGD5);
        SetGrenadeType(EGrenadeType.CHEMICAL);
        m_Pinned = false;
        SetPinnable(false);
        Arm();
