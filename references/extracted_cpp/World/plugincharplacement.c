// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper/plugincharplacement.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginCharPlacement extends PluginBase
{
    PluginDeveloper     m_ModuleDeveloper;
    PlayerBase          m_Character;
    ref                 Timer m_Timer;
    bool                m_Initialized;
    vector              m_cam_pos;
    vector              m_character_pos;
    vector              m_camera_dir;
    vector              m_cameraTrans[4];
    vector              m_demoPos;
    vector              m_demoRot;
    vector              m_camera_orientation;
    
    const float         FOV = 0.5236;
    
    void CheckInit()
    {
        if ( m_Initialized )
        {
            Print(&quot;logging...&quot;);
            Log(FOV.ToString(),&quot;FOV&quot;);
            Log(m_camera_dir.ToString(),&quot;camera dir&quot;);
            Log(m_cam_pos.ToString(),&quot;camera pos&quot;);
            Log(m_demoPos.ToString(),&quot;character pos&quot;);
            
            return;
        }
        
        if( FreeDebugCamera.GetInstance().IsActive() ) 
            Init();
