// Source: F:/Games/Dayz/scripts/4_world/entities/grenade_base/rdg2smokegrenade_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RDG2SmokeGrenade_ColorBase extends SmokeGrenadeBase
{
    const string SOUND_SMOKE_START = &quot;SmokegGrenades_RDG2_start_loop_SoundSet&quot;;
    const string SOUND_SMOKE_LOOP = &quot;SmokegGrenades_RDG2_active_loop_SoundSet&quot;;
    const string SOUND_SMOKE_END = &quot;SmokegGrenades_RDG2_end_loop_SoundSet&quot;;
    
    void RDG2SmokeGrenade_ColorBase()
    {
        SetAmmoType(&quot;&quot;);
        SetFuseDelay(2);
        SetParticlePosition(&quot;0 0.1 0&quot;);
        SetSoundSmokeStart(SOUND_SMOKE_START);
        SetSoundSmokeLoop(SOUND_SMOKE_LOOP);
        SetSoundSmokeEnd(SOUND_SMOKE_END);
