// Source: F:/Games/Dayz/scripts/5_mission/gui/ingamehud.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class IngameHud extends Hud
{
    protected const float                       FADE_IN_TIME = 0.3;
    protected const float                       FADE_OUT_TIME = 0.3;
    protected const float                       HIDE_MENU_TIME = 5;
 
    protected ref map<int,string>               m_StatesWidgetNames;
    protected ref map<int,ImageWidget>          m_StatesWidgets;  // [key] ImageWidget
 
    protected ref map<ImageWidget, int>         m_TendencyStatusCritical;   //array of icons that are blinking due to critical tendency status
    protected const float                       TENDENCY_BLINK_TIME = 0.25;
    protected float                             m_BlinkTime;
    
    protected ref map<int,string>               m_BadgesWidgetNames;
    protected ref map<int,int>                  m_BadgesSupportedLevels;
    protected ref map<int,int>                  m_BadgesWidgetDisplay;
    protected ref map<int,ImageWidget>          m_BadgesWidgets;  // [key] ImageWidget
    protected bool                              m_IsTemperatureVisible;
    protected float                             m_TemperatureTimer;
    protected float                             m_TemperatureShowTime = 30;
    protected ImageWidget                       m_HeatBufferPlus;
 
    protected bool                              m_IsStaminaVisible;
    protected float                             m_StaminaTimer;
    protected float                             m_StaminaShowTime = 0.15;
    
    protected Widget                            m_HudPanelWidget;       // always visible
    protected Widget                            m_QuickbarWidget;
    protected Widget                            m_VehicleHudPanels;
    protected ref InventoryQuickbar             m_Quickbar;
            
    protected VehicleHudBase                        m_ActiveVehicleHUD;
    protected ref map<string, ref VehicleHudBase>   m_VehicleHudMap; 
        
    protected Widget                            m_GameStatusIconsPanel;                 
    protected ImageWidget                       m_HighPingA;                    
    protected ImageWidget                       m_HighPingB;                    
    protected ImageWidget                       m_LowServerPerfA;           
    protected ImageWidget                       m_LowServerPerfB;           
    protected ImageWidget                       m_ConnectionLost;           
    
    protected Widget                            m_Notifiers;
    protected TextWidget                        m_BloodType;
    protected TextWidget                        m_BloodPosType;
    protected Widget                            m_Badges;
    protected ref Timer                         m_HideTimer;
    protected ref WidgetFadeTimer               m_FadeTimerCrosshair;
    protected ref WidgetFadeTimer               m_FadeTimerActionCursor;
//  protected ref WidgetFadeTimer               m_FadeTimerWeaponMode;
//  protected ref WidgetFadeTimer               m_FadeTimerZeroing;
    protected ref WidgetFadeTimer               m_FadeTimerZeroingOld;
    protected ref WidgetFadeTimer               m_FadeTimerMagazineName;
    protected ref WidgetFadeTimer               m_FadeTimerWeaponName;
    protected ref WidgetFadeTimer               m_FadeTimerWeaponStats;
    protected ref WidgetFadeTimer               m_FadeTimerQuickbar;
    protected ref WidgetFadeTimer               m_FadeTimerWalkieTalkie;
    protected ref WidgetFadeTimer               m_FadeTimerWalkieTalkieText;
    
    protected ProgressBarWidget                 m_Stamina;
    protected Widget                            m_StaminaBackground;
    protected Widget                            m_Presence;
    protected Widget                            m_StanceProne;
    protected Widget                            m_StanceStand;
    protected Widget                            m_StanceStandWalk;
    protected Widget                            m_StanceCrouch;
    protected Widget                            m_StanceCar;
    protected Widget                            m_PresenceLevel0;
    protected Widget                            m_PresenceLevel1;
    protected Widget                            m_PresenceLevel2;
    protected Widget                            m_PresenceLevel3;
    protected Widget                            m_PresenceLevel4;
    protected ref Timer                         myTimer; //TEMP
    protected ref array<ref WidgetFadeTimer>    m_FadeTimers = new array<ref WidgetFadeTimer>;
    protected Widget                            m_MouseSingleAction;
    protected Widget                            m_MouseContinuousAction;
    protected Widget                            m_RadialProgressBar1;
    protected Widget                            m_RadialProgressBarCrossHair;
    protected Widget                            m_RadialProgressBar;
    protected Widget                            m_CursorWidget;
    protected ImageWidget                       m_CursorIcon;
    protected Widget                            m_WalkieTalkie;
    
    protected Widget                            m_ActionWidget;
    protected TextWidget                        m_ActionDesc;
    protected TextWidget                        m_ActionItemDesc;
    protected Widget                            m_ActionItemQuantityWrapper
    protected ImageWidget                       m_ActionHealthMark;
    protected ProgressBarWidget                 m_ActionQuantityBar;
    protected Widget                            m_ActionQuantityBox;
    protected TextWidget                        m_ActionQuantityBoxText;
    protected ImageWidget                       m_ActionIcon;
    protected TextWidget                        m_ActionButtonText;
 
    protected Widget                            m_ActionIconFrame;
    protected Widget                            m_ActionMultipleItemsFrame;
    
    protected Widget                            m_ActionTarget;
    protected ActionTargetsCursor               m_ActionTargetsCursor;
    
    // CrossHairs
    protected ImageWidget                       m_PermanentCrossHair;
    protected bool                              m_ZeroingKeyPressed;
    
    protected Widget                            m_HitIndicatorUp;
    protected Widget                            m_HitIndicatorDown;
    protected Widget                            m_HitIndicatorLeft;
    protected Widget                            m_HitIndicatorRight;
    protected Widget                            m_HitIndicatorSingle;
    
    ref array<ref HitDirectionEffectBase>       m_HitDirEffectArray;
    
    protected ref IngameHudHeatBuffer           m_HeatBufferUI;
    protected ref IngameHudVisibility           m_HudVisibility;
    
    int         m_PlayerSpineIndex;
    PlayerBase  m_CurrentTaggedPlayer;
    Widget      m_PlayerTag;
    TextWidget  m_PlayerTagText;
        
    void IngameHud()
    {
        m_FadeTimerCrosshair            = new WidgetFadeTimer;
        m_FadeTimerActionCursor         = new WidgetFadeTimer;
        m_FadeTimerQuickbar         = new WidgetFadeTimer;
        m_FadeTimerWalkieTalkie     = new WidgetFadeTimer;
        m_FadeTimerWalkieTalkieText = new WidgetFadeTimer;
        
        m_StatesWidgets                 = new map<int, ImageWidget>; // [key] widgetName
        m_StatesWidgetNames             = new map<int, string>;
        
        m_TendencyStatusCritical        = new map<ImageWidget, int>;
        
        m_BadgesWidgets                 = new map<int, ImageWidget>; // [key] widgetName
        m_BadgesWidgetNames             = new map<int, string>;
        m_BadgesSupportedLevels         = new map<int, int>;
        m_BadgesWidgetDisplay           = new map<int, int>;
        
        m_VehicleHudMap                 = new map<string, ref VehicleHudBase>;
        
        m_HudVisibility                 = new IngameHudVisibility();
