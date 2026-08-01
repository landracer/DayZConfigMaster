// Source: F:/Games/Dayz/scripts/4_world/classes/softskillsmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SoftSkillsManager
{   
    protected PlayerBase            m_Player;
    protected float                 m_SpecialtyLevel;
    protected float                 m_RoughLevel;
    protected float                 m_PreciseLevel;
     
    protected bool                  m_IsLinear;
    protected bool                  m_IsActive;
    protected bool                  m_IsCoolDown;
 
    protected int                   m_UserActionsCounter;   
 
    static protected const int      DEFAULT_EFFICIENCY = 0;
    static protected const float    PRECISE_WEIGHT_LIMIT = -1; 
    static protected const float    ROUGH_WEIGHT_LIMIT = 1;
    static protected const float    COOLDOWN_TIMER = 5; //default 5, 
 
    protected ref Timer             m_CoolDownTimer = new Timer();
 
    protected bool              m_IsDebugMode;
    protected float                 m_CoolDownValue;
    protected float                 m_LastUAValue;
    protected float                 m_ComponentBonusBefore;
    protected float                 m_ComponentBonusAfter;
    protected float                 m_GeneralBonusBefore;
    protected float                 m_GeneralBonusAfter;
 
    protected ref SoftSkillManagerDebug m_DebugWindow;
 
    protected ref Timer             m_SynchTimer;
 
    void SoftSkillsManager( PlayerBase player )
    {
        m_Player = player;
        m_IsCoolDown = false;
        m_IsLinear = false;
        m_IsActive = true;
        m_IsDebugMode = false;
