// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginfilehandler/pluginlocalprofile/pluginlocalprofilescene.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
//Mission = &quot;ChernarusPlus&quot;
typedef CfgParamString DefCfgParamString
 
//InitTime = 220
typedef CfgParamInt DefCfgParamInt
 
//InitWeatherRain = 0.5
typedef CfgParamFloat DefCfgParamFloat
 
//InitWeatherFog = 0.3
typedef CfgParamFloat DefCfgParamFloat
 
//SceneObjectsCount = 1
typedef CfgParamInt DefCfgParamInt
*/
 
class PluginLocalProfileScene extends PluginLocalProfile
{
    private const string FILE_ROOT          = &quot;$saves:&quot;;
    private const string FILE_ROOT_SCENES   = &quot;Scenes&quot;;
    private const string PARAM_MISSION      = &quot;Mission&quot;;
    private const string PARAM_TIME         = &quot;InitTime&quot;;
    private const string PARAM_RAIN         = &quot;InitWeatherRain&quot;;
    private const string PARAM_FOG          = &quot;InitWeatherFog&quot;;
    private const string PARAM_OBJ_COUNT    = &quot;SceneObjectsCount&quot;;
    private const string PARAM_OBJ_NAME     = &quot;SceneObject&quot;;
    
    private string m_FileSceneName;
    
    //========================================
    // GetPathScenes
    //========================================
    string GetPathScenes()
    {
        return FILE_ROOT+&quot;\\&quot;+FILE_ROOT_SCENES;
