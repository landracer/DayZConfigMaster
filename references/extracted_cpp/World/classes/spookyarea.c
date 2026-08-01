// Source: F:/Games/Dayz/scripts/4_world/classes/contaminatedarea/spookyarea.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// The base area for Spooky land, spooky particles and spooky triggers for a spooky halloween
class SpookyArea : EffectArea
{
    // ----------------------------------------------
    //              INITIAL SETUP
    // ----------------------------------------------
    
    override void EEInit()
    {
        if (!m_ToxicClouds)
            m_ToxicClouds = new array<Particle>();
        
        SetSynchDirty();
        
        super.EEInit();
