// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/trapspawnbase/trap_smallfish.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Trap_SmallFish extends TrapSpawnBase
{
    override void InitTrapValues()
    {
        super.InitTrapValues();
        
        m_DefectRate = 2.5;             //Added damage after trap activation
        
        m_InitWaitTimeMin                       = 120;
        m_InitWaitTimeMax                       = 180;
        m_UpdateWaitTime                        = 30;
        m_SpawnUpdateWaitTime                   = 30;
        m_MaxActiveTime                         = 1200;
 
        m_MinimalDistanceFromPlayersToCatch     = 15;
        
        m_AnimationPhaseSet                     = &quot;inventory&quot;;
        m_AnimationPhaseTriggered               = &quot;placing&quot;;
        m_AnimationPhaseUsed                    = &quot;triggered&quot;;
