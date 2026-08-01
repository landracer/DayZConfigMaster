// Source: F:/Games/Dayz/scripts/5_mission/gui/itemactionswidget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ItemActionsWidget extends ScriptedWidgetEventHandler
{
    protected PlayerBase                m_Player;
    protected EntityAI                  m_EntityInHands;
    protected ActionBase                m_Interact;
    protected ActionBase                m_ContinuousInteract;
    protected ActionBase                m_Single;
    protected ActionBase                m_Continuous;
    protected ActionManagerClient       m_AM;
    protected IngameHud                 m_Hud;
    
    protected UAIDWrapper               m_UseActionWrapper;
 
    protected int                       m_InteractActionsNum;
    protected int                       m_ContinuousInteractActionsNum;
    protected int                       m_ItemActionsNum;
    protected int                       m_ContinuousItemActionsNum;
    protected bool                      m_HealthEnabled;
    protected bool                      m_QuantityEnabled;
    
    protected ref WidgetFadeTimer       m_FadeTimer;
    protected bool                      m_Faded;
    protected bool                      m_Hidden;
    protected bool                      m_ItemFrozen;
 
    protected Widget                    m_Root;
    protected Widget                    m_ItemLeft;

    protected float m_MaxWidthChild;
    protected float m_RootWidth;
    protected float m_RootHeight;
    
    void ItemActionsWidget()
    {
        m_Interact              = null;
        m_ContinuousInteract    = null;
        m_Single                = null;
        m_Continuous            = null;
        m_AM                    = null;
 
        m_FadeTimer             = new WidgetFadeTimer;
        m_Faded                 = true;
        
        m_HealthEnabled         = true;
        m_QuantityEnabled       = true;
        
        m_Hud                   = GetHud();
        m_Hidden                = true;
        m_ItemFrozen            = false;
        
        m_UseActionWrapper      = GetUApi().GetInputByID(UAAction).GetPersistentWrapper();
        
        g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
        g_Game.GetMission().GetOnInputPresetChanged().Insert(OnInputPresetChanged);
        g_Game.GetMission().GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
