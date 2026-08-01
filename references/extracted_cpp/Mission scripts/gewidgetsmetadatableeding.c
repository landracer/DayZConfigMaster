// Source: F:/Games/Dayz/scripts/5_mission/mission/gameplayeffectwidgets/gewidgetsmetadatableeding.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class GameplayEffectsDataBleeding extends GameplayEffectsData
{
    protected bool m_Initialized; //tied to initialization of &#39;BleedingSourcesManagerBase&#39; on player object, skips updates until ready, even when formally active
    protected bool m_Visible; //overall visibility
    protected ref map<int,ref BleedingIndicator> m_RegisteredInstances;
    protected ref array<int> m_CleanupQueue;
    protected ref array<int> m_RunningIndicators;
    protected int m_LastDropIdx;
    protected int m_ImageWidgetCount; //number of available blood drop image widgets
    protected ref map<int,ref array<float>> m_ProbabilityMap;
    protected ref array<Widget> m_PositioningFramesArray;
    protected Widget m_BloodDropsFrame;
    
    protected int m_LastPositionFrameUsed;
    
    void GameplayEffectsDataBleeding(array<ref Widget> input, int type, int user_override = -1)
    {
        m_RegisteredInstances = new map<int,ref BleedingIndicator>;
        m_CleanupQueue = new array<int>;
        m_RunningIndicators = new array<int>;
        m_Initialized = false;
