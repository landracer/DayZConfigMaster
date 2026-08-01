// Source: F:/Games/Dayz/scripts/5_mission/gui/sceneeditormenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SceneEditorMenu extends UIScriptedMenu
{   
//---------------------------------------------------------------------------------
// >> Public Scope
    static const int POPUP_ID_SCENE_MANAGER     = 0;
    static const int POPUP_ID_SCENE_SETTINGS    = 1;
    static const int POPUP_ID_SCENE_NEW         = 2;
    static const int POPUP_ID_SCENE_RENAME      = 3;
    static const int POPUP_ID_SCENE_DELETE      = 4;
    static const int POPUP_ID_NOTIFY            = 5;
    static const int POPUP_ID_EDITOR_SETTINGS   = 6;
    static const int POPUP_ID_INIT_SCRIPT       = 7;
    static const int POPUP_ID_POSITION_MANAGER      = 8;
    static const int POPUP_ID_PRESET_NEW = 9;
    static const int POPUP_ID_PRESET_RENAME = 10;
    static const int POPUP_ID_CONFIGS = 11;
    const string CONST_DEFAULT_PRESET_PREFIX = &quot;[Default]&quot;; 
    
    // Render specific Preset Items
    void RenderPresets()
    {
        m_PresetsTextListbox.ClearItems();
 
        int i;
        TBoolArray preset_params;
 
        // load fixed presets list
        TStringArray presets_array = m_ConfigDebugProfileFixed.GetPresets();
        for ( i = 0; i < presets_array.Count(); i++ )
        {
            m_PresetsTextListbox.AddItem( &quot;[&quot;+presets_array.Get(i)+&quot;]&quot;, new PresetParams ( presets_array.Get(i), true, false), 0);
        }
 
        // load custom presets list
        TStringArray custom_presets_array = m_ConfigDebugProfile.GetPresets();
        for ( i = 0; i < custom_presets_array.Count(); i++ )
        {
            m_PresetsTextListbox.AddItem( custom_presets_array.Get(i), new PresetParams ( custom_presets_array.Get(i),false, false), 0);
        }
 
        string default_preset = m_ConfigDebugProfile.GetDefaultPreset();
        if ( default_preset != &quot;&quot; )
        {
            // if is fixed
            int index = GetPresetIndexByName( default_preset );
            if ( IsPresetFixed( default_preset) )
            {
                default_preset = &quot;[&quot; + default_preset + &quot;]&quot;;
            }
            PresetParams preset_params_array;
            if( index > -1 && index < m_PresetsTextListbox.GetNumItems() )
            {
                m_PresetsTextListbox.GetItemData( index, 0, preset_params_array );
                m_PresetsTextListbox.SetItem( index, default_preset + CONST_DEFAULT_PRESET_PREFIX, preset_params_array, 0 );
            }
        }
