// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginfilehandler/pluginconfighandler/pluginconfigscene.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginConfigScene extends PluginConfigHandler
{
    protected string FILE_ROOT                  = &quot;$saves:&quot;;
    protected const string FILE_ROOT_SCENES     = &quot;scenes&quot;;
    
    protected const string PARAM_MISSION        = &quot;Mission&quot;;
    protected const string PARAM_TIME           = &quot;InitTime&quot;;
    protected const string PARAM_YEAR           = &quot;InitYear&quot;;
    protected const string PARAM_MONTH          = &quot;InitMonth&quot;;
    protected const string PARAM_DAY            = &quot;InitDay&quot;;
    protected const string PARAM_HOUR           = &quot;InitHour&quot;;
    protected const string PARAM_MINUTE         = &quot;InitMinute&quot;;
    protected const string PARAM_OVERCAST       = &quot;WeatherInitOvercast&quot;;
    protected const string PARAM_RAIN           = &quot;WeatherInitRain&quot;;
    protected const string PARAM_FOG            = &quot;WeatherInitFog&quot;;
    protected const string PARAM_WIND_F         = &quot;WeatherInitWindForce&quot;;
    protected const string PARAM_PLAYER         = &quot;Player&quot;;
    protected const string PARAM_OBJ_COUNT      = &quot;SceneObjectsCount&quot;;
    protected const string PARAM_OBJ_NAME       = &quot;SceneObject&quot;;
    protected const string PARAM_OBJ_ATT        = &quot;Att&quot;;
    protected const string PARAM_OBJ_LNK        = &quot;Lnk&quot;;
    protected const string PARAM_OBJ_TYPE       = &quot;type&quot;;
    protected const string PARAM_OBJ_POS        = &quot;position&quot;;
    protected const string PARAM_OBJ_ROT        = &quot;rotation&quot;;
    protected const string PARAM_OBJ_DMG        = &quot;damage&quot;;
    protected const string PARAM_OBJ_HLT        = &quot;health&quot;;
    protected const string PARAM_OBJ_ISCR       = &quot;init_script&quot;;
    
    TStringArray m_CfgTemplate;
    
    protected string m_FileSceneName;
 
    //========================================
    // OnInit
    //========================================
    override void OnInit()
    {
        //super.OnInit();
            
        FILE_ROOT = g_Game.GetMissionFolderPath();
