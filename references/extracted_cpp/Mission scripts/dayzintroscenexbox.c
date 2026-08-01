// Source: F:/Games/Dayz/scripts/5_mission/dayzintroscenexbox.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZIntroSceneXbox: Managed
{
    protected bool m_IsCharFemale;
    protected int m_LastPlayedCharacterID;
    
    protected ref TStringArray  m_genderList;
    protected ref TStringArray  m_CharPersonalityMaleList;
    protected ref TStringArray  m_CharPersonalityFemaleList;
    protected ref TStringArray  m_CharShirtList;
    protected ref TStringArray  m_CharPantsList;
    protected ref TStringArray  m_CharShoesList;
    protected ref TStringArray  m_AllCharacters;
    
    protected ref IntroSceneCharacter   m_Character;
    protected Camera                    m_SceneCamera;
    protected Weather                   m_Weather;
    protected Object                    m_Clutter;
    protected vector                    m_CharacterPos;
    protected vector                    m_CharacterDir;
    protected ref TIntArray             m_Date = new TIntArray;
 
    protected MenuData                  m_MenuData;
    
    protected ref MenuCarEngineSmoke    m_FXParticleCarSmoke;
    protected ref MenuEvaporation       m_FXParticleStreamLeft;
    protected ref MenuEvaporation       m_FXParticleStreamRight;
    
    ref Timer m_TimerUpdate = new Timer( CALL_CATEGORY_GAMEPLAY );
    ref Timer m_TimerParticle = new Timer( CALL_CATEGORY_GAMEPLAY );
    ref Timer m_TimerDate = new Timer( CALL_CATEGORY_GAMEPLAY );
    ref Timer m_TimerClientUpdate = new Timer( CALL_CATEGORY_GAMEPLAY );
 
    //==================================
    // DayZIntroSceneXbox
    //==================================
    void DayZIntroSceneXbox()
    {
        m_MenuData = g_Game.GetMenuData();
        
        m_LastPlayedCharacterID = m_MenuData.GetLastPlayedCharacter();
        m_CharacterPos = &quot;0 0 0&quot;;
        m_CharacterDir = &quot;0 0 0&quot;;
        
        //g_Game.m_PlayerName = &quot;Survivor&quot;; //default
        if ( m_MenuData.GetCharactersCount() == 0 )
        {
            m_LastPlayedCharacterID = -1;
        }
        
        if ( m_LastPlayedCharacterID > -1 )
        {
            string characterName = g_Game.GetPlayerGameName();
            m_MenuData.GetCharacterName(m_LastPlayedCharacterID, characterName);
            g_Game.SetPlayerGameName(characterName);
        }
    
        // Camera Setup
        vector camera_position;
        camera_position[0]          = 1323.0;   // X
        camera_position[1]          = 1.0;      // Y
        camera_position[2]          = 1590.37;  // Z
        float camera_rotation_h     = 100;
        float camera_rotation_v     = -3;
        float camera_fov            = 0.85;
        float camera_focus_distance = 0.0;
        float camera_focus_streght  = 0.0;
        
        // Character
        float character_distance = 2.25;
        
        // Camera Setup
        m_SceneCamera = CameraCreate(camera_position, camera_rotation_h, camera_rotation_v, camera_fov, camera_focus_distance, camera_focus_streght);
        m_SceneCamera.SetActive(true);
        
        PPEffects.Init(); //Deprecated, left in for legacy purposes only
        
        // Character Setup
        vector cam_dir = m_SceneCamera.GetDirection();
        m_CharacterPos = camera_position + ( cam_dir * character_distance );
        m_CharacterPos[1] = g_Game.SurfaceY(m_CharacterPos[0], m_CharacterPos[2]);
        m_CharacterDir = (camera_position - m_CharacterPos);
        
        float overcast = 0.42;
        float rain = 0.0;
        float snowfall = 0.0;
        float fog = 0.0;
        
 
        m_Weather = g_Game.GetWeather();
        m_Weather.GetOvercast().SetLimits(overcast, overcast);
        m_Weather.GetRain().SetLimits(rain, rain);
        m_Weather.GetSnowfall().SetLimits(snowfall, snowfall);
        m_Weather.GetFog().SetLimits(fog, fog);
        
        m_Weather.GetOvercast().Set(overcast, 0, 0);
        m_Weather.GetRain().Set(rain, 0, 0);
        m_Weather.GetSnowfall().Set(snowfall, 0, 0);
        m_Weather.GetFog().Set(fog, 0, 0);
        
        m_Character = new IntroSceneCharacter();
        m_Character.LoadCharacterData(m_CharacterPos, m_CharacterDir);
        
        m_TimerParticle.Run(0.1, this, &quot;SetupParticles&quot;, null, false);
        //m_TimerDate.Run(2.0, this, &quot;SetupDate&quot;, null, false);
        m_TimerUpdate.Run(0.5, this, &quot;SetupCharacter&quot;, null, true);
        
        vector clut_pos = SnapToGround( m_CharacterPos + &quot;-1 0 0&quot; );
        m_Clutter = g_Game.CreateObject( &quot;ClutterCutter2x2&quot;, clut_pos, true );
        
        // Xbox check update
        CheckXboxClientUpdateLoopStart();
        
        g_Game.SetHudBrightness(g_Game.GetHUDBrightnessSetting());
        
        g_Game.GetCallQueue(CALL_CATEGORY_GUI).Call(SetInitPostprocesses);
