// Source: F:/Games/Dayz/scripts/4_world/entities/explosivesbase/claymoremine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ClaymoreMine : ExplosivesBase
{
    protected const string ANIM_PHASE_PACKED    = &quot;Bag&quot;;
    protected const string ANIM_PHASE_DEPLOYED  = &quot;Mine&quot;;
    protected const string SELECTION_NAME_LED   = &quot;LED&quot;;
 
    protected ref RemotelyActivatedItemBehaviour    m_RAIB;
    protected ERemoteDetonatorLEDState              m_LastLEDState;
    
    void ClaymoreMine()
    {
        m_RAIB = new RemotelyActivatedItemBehaviour(this);
        
        SetAmmoTypes({&quot;ClaymoreMine_Ammo&quot;,&quot;ClaymoreMine_Secondary_Ammo&quot;});
        SetParticleExplosion(ParticleList.CLAYMORE_EXPLOSION);
        SetParticleOrientation(&quot;90 0 0&quot;);
 
        RegisterNetSyncVariableInt(&quot;m_RAIB.m_PairDeviceNetIdLow&quot;);
        RegisterNetSyncVariableInt(&quot;m_RAIB.m_PairDeviceNetIdHigh&quot;);
        UpdateLED(ERemoteDetonatorLEDState.OFF, true);
