// Source: F:/Games/Dayz/scripts/4_world/entities/building/crashbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CrashBase extends House
{
    Particle m_ParticleEfx;
    static bool m_Init = Init();
    
    static bool Init()
    {
        CrashSoundSets.RegisterSoundSet(&quot;HeliCrash_Distant_SoundSet&quot;);
        CrashSoundSets.RegisterSoundSet(&quot;SledgeCrash_Distant_SoundSet&quot;);
        return true;
