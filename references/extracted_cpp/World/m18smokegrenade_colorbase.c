// Source: F:/Games/Dayz/scripts/4_world/entities/grenade_base/m18smokegrenade_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class M18SmokeGrenade_ColorBase extends SmokeGrenadeBase
{
    const string SOUND_SMOKE_START = &quot;SmokegGrenades_M18_start_loop_SoundSet&quot;;
    const string SOUND_SMOKE_LOOP = &quot;SmokegGrenades_M18_active_loop_SoundSet&quot;;
    const string SOUND_SMOKE_END = &quot;SmokegGrenades_M18_end_loop_SoundSet&quot;;
 
    void M18SmokeGrenade_ColorBase()
    {
        SetAmmoType(&quot;&quot;);
        SetFuseDelay(2);
        SetSoundSmokeStart(SOUND_SMOKE_START);
        SetSoundSmokeLoop(SOUND_SMOKE_LOOP);
        SetSoundSmokeEnd(SOUND_SMOKE_END);
