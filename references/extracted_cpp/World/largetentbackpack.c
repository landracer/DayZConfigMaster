// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/tentbase/largetentbackpack.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LargeTentBackPack extends Backpack_Base
{       
    ref protected EffectSound   m_RepackingLoopSound;   
    
    void LargeTentBackPack()
    {
        m_RepackingLoopSound  = new EffectSound;    
