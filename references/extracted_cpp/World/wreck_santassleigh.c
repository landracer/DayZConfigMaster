// Source: F:/Games/Dayz/scripts/4_world/entities/building/wrecks/wreck_santassleigh.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//Christmas Event: Santa&#39;s Sleigh
class Wreck_SantasSleigh extends CrashBase
{
    XmasSleighLight         m_SleighLight;
    
    int m_MaxDeersAmount = 4;
    int m_MinDeersAmount = 2;
    int m_MaxDeersSpawnRange = 25;
    int m_MinDeersSpawnRange = 5;
 
    void Wreck_SantasSleigh()
    {
        if (!g_Game.IsDedicatedServer())
        {
            //particles - Aurora trail
            m_ParticleEfx = ParticleManager.GetInstance().PlayOnObject(ParticleList.AURORA_SANTA_WRECK,this,vector.Zero,vector.Zero,true);
            
            //lights - green light
            m_SleighLight = XmasSleighLight.Cast(ScriptedLightBase.CreateLight(XmasSleighLight,vector.Zero));
            m_SleighLight.AttachOnMemoryPoint(this, &quot;light&quot;);
        }
