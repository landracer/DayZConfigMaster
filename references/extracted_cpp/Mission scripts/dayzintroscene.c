// Source: F:/Games/Dayz/scripts/5_mission/dayzintroscene.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZIntroScene : Managed
{
    protected ref IntroSceneCharacter   m_Character;
    protected Camera                    m_Camera;
    protected vector                    m_CameraTrans[4];
    protected vector                    m_CharacterPos;
    protected vector                    m_CharacterRot;
    protected Weather                   m_Weather;  
    protected vector                    m_Target;
 
    protected ref OptionsMenu   m_OptionsMenu;
 
    void DayZIntroScene()
    {
        string root_path = &quot;cfgCharacterScenes &quot; + g_Game.GetWorldName();
        
        int count = g_Game.ConfigGetChildrenCount(root_path);
        int index = Math.RandomInt(0, count - 1);
        string childName;
        g_Game.ConfigGetChildName(root_path, index, childName);
    
        string scene_path = root_path + &quot; &quot; + childName;
        m_Target = SwapYZ(g_Game.ConfigGetVector(scene_path + &quot; target&quot;));
        vector position = SwapYZ(g_Game.ConfigGetVector(scene_path + &quot; position&quot;));
        TIntArray date = new TIntArray;
        TFloatArray storm = new TFloatArray;
        g_Game.ConfigGetIntArray(scene_path + &quot; date&quot;, date);
        float fov = g_Game.ConfigGetFloat(scene_path + &quot; fov&quot;);
        float overcast = g_Game.ConfigGetFloat(scene_path + &quot; overcast&quot;);
        float rain = g_Game.ConfigGetFloat(scene_path + &quot; rain&quot;);
        float snowfall = g_Game.ConfigGetFloat(scene_path + &quot; snowfall&quot;);
        float fog = g_Game.ConfigGetFloat(scene_path + &quot; fog&quot;);
        float windspeed = -1;
        if ( g_Game.ConfigIsExisting(scene_path + &quot; windspeed&quot;) )   windspeed = g_Game.ConfigGetFloat(scene_path + &quot; windspeed&quot;);
        g_Game.ConfigGetFloatArray(scene_path + &quot; storm&quot;, storm);
        
        World world = g_Game.GetWorld();
    
        if (world && date.Count() >= 5)
        {
            world.SetDate(date.Get(0), date.Get(1), date.Get(2), date.Get(3), date.Get(4));
        }
    
        g_Game.ObjectDelete( m_Camera );
        Class.CastTo(m_Camera, g_Game.CreateObject(&quot;staticcamera&quot;, SnapToGround(position), true)); //Vector(position[0] , position[1] + 1, position[2])
        
        Math3D.MatrixIdentity4(m_CameraTrans);
        
        if (m_Camera)
        {
            //m_Camera.SetPosition(Vector(m_Camera.GetPosition()[0],m_Camera.GetPosition()[1]+0,m_Camera.GetPosition()[2]));
            m_Camera.LookAt(m_Target);
            m_Camera.SetFOV(fov);
            m_Camera.SetFocus(5.0, 0.0); //5.0, 1.0
            
            m_Camera.SetActive(true);
            
            Math3D.DirectionAndUpMatrix(m_Target - SnapToGround(position), &quot;0 1 0&quot;, m_CameraTrans);
            m_CameraTrans[3] = m_Camera.GetPosition();
            m_CharacterPos = Vector(0.685547, -0.988281, 3.68823).Multiply4(m_CameraTrans);
 
            float pos_x = m_CharacterPos[0];
            float pos_z = m_CharacterPos[2];
            float pos_y = g_Game.SurfaceY(pos_x, pos_z);
            vector ground_demo_pos = Vector(pos_x, pos_y, pos_z);
            m_CharacterPos = ground_demo_pos;
 
            vector to_cam_dir = m_Camera.GetPosition() - m_CharacterPos;
            m_CharacterRot[0] = Math.Atan2(to_cam_dir[0], to_cam_dir[2]) * Math.RAD2DEG;
        }
        
        m_Weather = g_Game.GetWeather();
        m_Weather.GetOvercast().SetLimits( overcast, overcast );
        m_Weather.GetRain().SetLimits( rain, rain );
        m_Weather.GetSnowfall().SetLimits( snowfall, snowfall );
        m_Weather.GetFog().SetLimits( fog, fog );
        
        m_Weather.GetOvercast().Set( overcast, 0, 0);
        m_Weather.GetRain().Set( rain, 0, 0);
        m_Weather.GetSnowfall().Set( snowfall, 0, 0);
        m_Weather.GetFog().Set( fog, 0, 0);
        
        if ( storm.Count() == 3 )
        {
            m_Weather.SetStorm(storm.Get(0),storm.Get(1),storm.Get(2));
        }
        
        if ( windspeed != -1 )
        {
            m_Weather.SetWindSpeed(windspeed);
            m_Weather.SetWindMaximumSpeed(windspeed);
            m_Weather.SetWindFunctionParams(1,1,1);
        }
        
        m_Character = new IntroSceneCharacter();
        m_Character.LoadCharacterData(m_CharacterPos, m_CharacterRot);
        
        PPEffects.Init(); //Deprecated, left in for legacy purposes only
        
        g_Game.GetCallQueue(CALL_CATEGORY_GUI).Call(SetInitPostprocesses);
