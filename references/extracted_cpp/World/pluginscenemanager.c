// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginscenemanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginSceneManager extends PluginBase
{
    static string       SCENE_DEFAULT_NAME      = &quot;default&quot;;
    static string       SCENE_SUFIX             = &quot;scene&quot;;
    static PlayerBase   PLAYER;
    static PluginSceneManager instance;
    
    bool            m_RulerActivated;
    ref array<vector>   m_RulerPoints;
    ref array<Shape>    m_RulerLines;
    ref array<Shape>    m_RulerSpheres;
    
    static const int SCENE_EDITOR_CMD_REFRESH   = 0;
    static const int SCENE_EDITOR_CMD_POPUP     = 1;
    static const int SCENE_EDITOR_CMD_SAVE      = 2;
    
    //---------------------------------------------------------------------------------
    // >> Public scope
 
    // System Events
    void PluginSceneManager()
    {
        if ( instance == NULL )
        {
            instance = this;
        }
