// Source: F:/Games/Dayz/scripts/5_mission/gui/ingamehudheatbuffer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//#define HEATBUFFER_INDICATOR_DEBUG // Uncomment for heat buffer indicator debug logs
class IngameHudHeatBuffer
{
    protected Widget m_Root;
 
    protected float m_CurrentValue;
    protected bool m_IsActive;
    protected int m_CurrentDirection; // -1 = none | 0 = decreasing | 1 = increasing
    protected int m_PreviousDirection; // -1 = none | 0 = decreasing | 1 = increasing
    protected bool m_Update;
    protected PlayerBase m_Player;
    protected float m_EffectTime;
    protected float m_EffectDuration;
    protected bool m_FadeIn;
    protected float m_FlashingTime;
    protected int m_FlashingStage;
    protected ref map<int, float> m_FlashingThresholds;
    
    void IngameHudHeatBuffer(Widget root, notnull PlayerBase player)
    {
        m_Root = root;
        m_Player = player;
        
        player.GetOnDeathStart().Insert(OnPlayerNegativeCondition);
        player.GetOnUnconsciousStart().Insert(OnPlayerNegativeCondition);
        player.GetOnUnconsciousStop().Insert(OnPlayerConditionChanged);
 
        m_CurrentDirection = -1;
        m_PreviousDirection = -1;
        m_Update = true;
        m_FlashingStage = -1;
        
        m_FlashingThresholds = new map<int, float>;
        m_FlashingThresholds.Insert(1, 0.002);
        m_FlashingThresholds.Insert(2, 0.332);
        m_FlashingThresholds.Insert(3, 0.662);
