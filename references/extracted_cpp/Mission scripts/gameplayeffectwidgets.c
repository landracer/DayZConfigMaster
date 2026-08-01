// Source: F:/Games/Dayz/scripts/5_mission/mission/gameplayeffectwidgets/gameplayeffectwidgets.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
TODO - doxygen formating
*/

class GameplayEffectWidgets extends GameplayEffectWidgets_base
{
    protected ref Widget                                m_Root; //dummy parent node
    protected ref map<int,ref Widget>                   m_Layouts;
    protected ref set<ref Widget>                       m_UniqueLayouts;
    protected ref GameplayEffectDataMap                 m_WidgetDataMap;
    protected ref set<int>                              m_RunningEffects;
    protected ref set<int>                              m_RunningEffectsPrevious;
    protected ref array<int>                            m_UpdatingEffects;
    protected ref array<ref Widget>                     m_UpdatedWidgetsCheck; //to make sure widgets are not updated over and over (case of multiple IDs sharing same widget set)
    protected ref array<int>                            m_UpdatedWidgetSetsCheck; //to make sure sets are not updated over and over (case of multiple IDs sharing same widget set)
    protected ref set<int>                              m_SuspendRequests;
    protected ref map<int,typename>                     m_IDToTypeMap;
    
    protected float                                     m_TimeProgBreath;
    protected float                                     m_BreathMultStamina;
    protected float                                     m_BreathResidue;
    
    //UserID&#39;s for widget containers that use something different from &#39;EffectWidgetsTypes&#39; defaults
    protected const int                                 WIDGETSET_BREATH = 100;
    
    //effect values
    protected int                                       m_BreathColor;
    protected float                                     m_BreathAlphaVal;
    protected float                                     m_FlashbangCoverAlphaVal;
    
    void GameplayEffectWidgets()
    {
        m_Root = g_Game.GetWorkspace().CreateWidget(FrameWidgetTypeID,0,0,1.0,1.0,WidgetFlags.VISIBLE | WidgetFlags.HEXACTPOS | WidgetFlags.VEXACTPOS, 0xffffffff, 0);
        m_Layouts = new map<int,ref Widget>;
        m_UniqueLayouts = new set<ref Widget>;
        m_WidgetDataMap = new GameplayEffectDataMap;
        m_RunningEffects = new set<int>;
        m_RunningEffectsPrevious = new set<int>;
        m_UpdatingEffects = new array<int>;
        m_UpdatedWidgetsCheck = new array<ref Widget>;
        m_UpdatedWidgetSetsCheck = new array<int>;
        m_SuspendRequests = new set<int>;
        m_IDToTypeMap = new map<int,typename>;
        
        m_TimeProgBreath = 0.0;
        m_BreathMultStamina = 1.0;
        
        Init();
        
        UpdateVisibility();
