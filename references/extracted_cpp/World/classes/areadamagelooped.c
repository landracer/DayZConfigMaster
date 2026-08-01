// Source: F:/Games/Dayz/scripts/4_world/classes/areadamage/areadamagenew/areadamagelooped.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AreaDamageLooped : AreaDamageManager
{
    protected float             m_CurrentTime = 0.0;
    protected float             m_PreviousTime = 0.0;   
    protected float             m_AccumulatedTime = 0.0;    
    protected float             m_LoopInterval = 1.0;
    
    protected int               m_AmountOfLoops = 0;
    
    protected bool              m_LoopByObject = true;
    
    void AreaDamageLooped(EntityAI parent, bool loopByObject = true)
    {
        m_LoopByObject = loopByObject;
