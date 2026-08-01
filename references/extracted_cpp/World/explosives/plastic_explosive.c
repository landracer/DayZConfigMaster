// Source: F:/Games/Dayz/scripts/4_world/entities/explosivesbase/plastic_explosive.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Plastic_Explosive : ExplosivesBase
{
    protected const string SLOT_TRIGGER                     = &quot;TriggerRemoteDetonator_Receiver&quot;;
    protected const string ANIM_PHASE_TRIGGER_REMOTE        = &quot;TriggerRemote&quot;;
    
    protected bool m_UsedAsCharge;
    
    protected ref RemotelyActivatedItemBehaviour m_RAIB;
 
    void Plastic_Explosive()
    {
        m_RAIB = new RemotelyActivatedItemBehaviour(this);
        
        SetAmmoType(&quot;Plastic_Explosive_Ammo&quot;);
        SetParticleExplosion(ParticleList.PLASTIC_EXPLOSION);
 
        RegisterNetSyncVariableInt(&quot;m_RAIB.m_PairDeviceNetIdLow&quot;);
        RegisterNetSyncVariableInt(&quot;m_RAIB.m_PairDeviceNetIdHigh&quot;);
