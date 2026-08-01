// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginfilehandler/pluginconfighandler/pluginconfigemotesprofile.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginConfigEmotesProfile extends PluginConfigHandler
{   
    /*protected const string EMOTE_1                = &quot;emote_slot_1&quot;;
    protected const string EMOTE_2              = &quot;emote_slot_2&quot;;
    protected const string EMOTE_3              = &quot;emote_slot_3&quot;;
    protected const string EMOTE_4              = &quot;emote_slot_4&quot;;
    protected const string EMOTE_5              = &quot;emote_slot_5&quot;;
    protected const string EMOTE_6              = &quot;emote_slot_6&quot;;
    protected const string EMOTE_7              = &quot;emote_slot_7&quot;;
    protected const string EMOTE_8              = &quot;emote_slot_8&quot;;
    protected const string EMOTE_9              = &quot;emote_slot_9&quot;;
    protected const string EMOTE_10             = &quot;emote_slot_10&quot;;
    protected const string EMOTE_11             = &quot;emote_slot_11&quot;;
    protected const string EMOTE_12             = &quot;emote_slot_12&quot;;*/
 
    protected ref map<string, ref CfgParam>     m_DefaultValues;
    protected ref TStringArray                  m_PresetList;   
    
    //========================================
    // GetInstance
    //========================================
    static PluginConfigEmotesProfile GetInstance()
    {
        return PluginConfigEmotesProfile.Cast( GetPlugin(PluginConfigEmotesProfile) );
