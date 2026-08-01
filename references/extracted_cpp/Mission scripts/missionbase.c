// Source: F:/Games/Dayz/scripts/5_mission/mission/missionbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MissionBase extends MissionBaseWorld
{
    PluginDeveloper         m_ModuleDeveloper;
    PluginKeyBinding        m_ModuleKeyBinding
    
    ref WidgetEventHandler  m_WidgetEventHandler;
    ref WorldData           m_WorldData;
    ref WorldLighting       m_WorldLighting;
    
    ref array<PlayerBase> m_DummyPlayers = new array<PlayerBase>;
 
    autoptr ObjectSnapCallback m_InventoryDropCallback;
    
    protected ref DynamicMusicPlayer m_DynamicMusicPlayer;
    protected ref DynamicMusicPlayerRegistry m_DynamicMusicPlayerRegistry;
 
    void MissionBase()
    {
        SetDispatcher(new DispatcherCaller);
        
        PluginManagerInit();
 
        m_WidgetEventHandler = new WidgetEventHandler();
 
        m_InventoryDropCallback = new EntityPlacementCallback();
        SoundSetMap.Init();
        
        GetDayZGame().GetYieldDataInitInvoker().Insert(InitWorldYieldDataDefaults);
        InitialiseWorldData();
        
        #ifndef SERVER
        GetDayZGame().GetAnalyticsClient().RegisterEvents();
        m_WorldLighting = new WorldLighting();
        
        m_DynamicMusicPlayer = new DynamicMusicPlayer(m_DynamicMusicPlayerRegistry);
            #ifdef DIAG_DEVELOPER
            GetOnTimeChanged().Insert(m_DynamicMusicPlayer.SetTimeOfDate);
            #endif
        AmmoTypesAPI.Init();
        #endif
        if (g_Game.IsServer() || !g_Game.IsMultiplayer())
        {
            OutdoorThermometerManager.Init();
        }
        GetOnInputDeviceConnected().Insert(UpdateInputDevicesAvailability);
        GetOnInputDeviceDisconnected().Insert(UpdateInputDevicesAvailability);
        
        // There is a possibility different maps/servers may be using different effects
        SEffectManager.Cleanup();
        GetUApi().UpdateControls();
