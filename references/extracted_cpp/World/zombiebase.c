// Source: F:/Games/Dayz/scripts/4_world/entities/creatures/infected/zombiebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ZombieBase extends DayZInfected
{
    const float TARGET_CONE_ANGLE_CHASE = 20;
    const float TARGET_CONE_ANGLE_FIGHT = 30;
    const float ORIENTATION_SYNC_THRESHOLD = 30; //threshold for local heading/orientation sync
    
    const float SHOCK_TO_STUN_MULTIPLIER = 2.82;

    protected int m_StanceVariation = 0;
    protected int m_LastMindState = -1;
    protected float m_LastMovementSpeed = -1;
    
    protected bool m_KnuckleLand = false;
    protected float m_KnuckleOutTimer = 0;
 
    protected int m_MindState = -1;
    protected int m_OrientationLocal = -1; //local &#39;companion&#39; value for sync checking
    protected int m_OrientationSynced = -1;
    protected float m_OrientationTimer;
    protected float m_MovementSpeed = -1;
    
    protected vector m_DefaultHitPosition;
    protected float m_DeltaTime;
    
    protected AbstractWave m_LastSoundVoiceAW;
    protected ref InfectedSoundEventHandler m_InfectedSoundEventHandler;
 
    protected ref array<Object> m_AllTargetObjects;
    protected ref array<typename>m_TargetableObjects;
    
    //static ref map<int,ref array<string>>     m_FinisherSelectionMap; //! which selections in the FireGeometry trigger which finisher on hit (when applicable)
    
    protected bool m_IsCrawling; //&#39;DayZInfectedCommandCrawl&#39; is transition to crawl only, &#39;DayZInfectedCommandMove&#39; used after that, hence this VARIABLE_WET
    
    protected bool m_FinisherInProgress = false; //is this object being backstabbed?
    
    protected ref ArrowManagerBase m_ArrowManager;
 
 
 
    //-------------------------------------------------------------
    void ZombieBase()
    {
        Init();
