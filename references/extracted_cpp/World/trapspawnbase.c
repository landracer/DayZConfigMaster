// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/trapspawnbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TrapSpawnBase extends ItemBase
{
    bool                            m_IsFoldable;
    bool                            m_CanCatch = false;
    
    //configurable stuff
    bool                            m_NeedInstalation;
    int                             m_InitWaitTimeMin;
    int                             m_InitWaitTimeMax;
    int                             m_UpdateWaitTime;           
    int                             m_SpawnUpdateWaitTime;      
    int                             m_MaxActiveTime;            
    float                           m_BaitLossFraction;         
    float                           m_DefectRate;               
    float                           m_MinimalDistanceFromPlayersToCatch; 
    
    //derived stuff
    private int                     m_InitWaitTime;             
    private int                     m_AdjustedMaxActiveTime;    
    private int                     m_ElapsedTime;
    private int                     m_ActivationTime;
    private int                     m_RollSuccessTime;
    private float                   m_CurrentMinimalDistance;
    private float                   m_CurrentlyUsedDelta;
    private bool                    m_IsCatchSuccessful;
    private int                     m_CatchEnviroMask = 0;
    private int                     m_YieldItemIdxLocal = -1;
    private int                     m_YieldItemIdx = -1;
    private int                     m_CatchParticleEffecterId = -1;
    
    vector                          m_PreyPos; // The position where prey will be spawned -> Will be overriden later
    
    protected bool                  m_IsActive;
    protected bool                  m_IsPastWaitingTime;
    protected bool                  m_IsDeployed;
    
    ref Timer                       m_Timer;
    
    string                          m_AnimationPhaseSet;
    string                          m_AnimationPhaseTriggered;
    string                          m_AnimationPhaseUsed;
 
    protected ref array<string>     m_PlaceableWaterSurfaceList;
    
    protected ref CatchingContextTrapsBase  m_CatchingContext;
    
    #ifdef DEVELOPER
    int m_dbgAttemptCount = 0;
    #endif
    
    void TrapSpawnBase()
    {
        InitTrapValues();
        
        RegisterNetSyncVariableBool(&quot;m_IsActive&quot;);
        RegisterNetSyncVariableBool(&quot;m_IsDeployed&quot;);
        RegisterNetSyncVariableInt(&quot;m_YieldItemIdx&quot;);
        
        //DEPRECATED stuff below, legacy reasons only
        m_CatchesPond                           = new multiMap<string, float>; //yields now in WorldData.InitYieldBank
        m_CatchesSea                            = new multiMap<string, float>; //yields now in WorldData.InitYieldBank
        m_CatchesGroundAnimal                   = new multiMap<string, float>; //yields now in WorldData.InitYieldBank
