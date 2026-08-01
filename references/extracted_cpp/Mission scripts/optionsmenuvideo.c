// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/options/optionsmenuvideo.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionsMenuVideo extends ScriptedWidgetEventHandler
{
    protected Widget                        m_Root;
    
    protected Widget                        m_SettingsRoot;
    protected Widget                        m_DetailsRoot;
    protected Widget                        m_DetailsBodyDefault;
    protected Widget                        m_DetailsBodyConnectivity;
    protected TextWidget                    m_DetailsLabel;
    protected RichTextWidget                m_DetailsText;
    
    protected GameOptions                   m_Options;
    protected OptionsMenu                   m_Menu;
    
    #ifdef PLATFORM_PS4
        protected ref OptionSelectorMultistate  m_FlipModeSelector;
    #else
        #ifndef PLATFORM_XBOX
            //Overall
            protected ref OptionSelectorMultistate  m_OverallQualitySelector;
            
            //Screen
            protected ref OptionSelectorMultistate  m_DisplayModeSelector;
            protected ref OptionSelectorMultistate  m_ResolutionSelector;
            protected ref OptionSelectorSlider      m_BrightnessSelector;
            protected ref OptionSelectorMultistate  m_VSyncSelector;
            //protected ref OptionSelectorMultistate    m_ColorDepthSelector;
            
            //Scene
            protected ref OptionSelectorMultistate  m_ObjectDetailSelector;
            protected ref OptionSelectorMultistate  m_TerrainDetailSelector;
            protected ref OptionSelectorMultistate  m_TextureDetailSelector;
            protected ref OptionSelectorMultistate  m_ShadowDetailSelector;
            protected ref OptionSelectorMultistate  m_VisibilitySelector;
            protected ref OptionSelectorMultistate  m_ObjectVisibilitySelector;
            
            //Rendering
            protected ref OptionSelectorMultistate  m_TextureFilteringSelector;
            protected ref OptionSelectorMultistate  m_TerrainSurfaceDetailSelector;
            protected ref OptionSelectorMultistate  m_PPAASelector;
            protected ref OptionSelectorMultistate  m_HWAASelector;
            protected ref OptionSelectorMultistate  m_ATOCSelector;
            protected ref OptionSelectorMultistate  m_AOSelector;
            protected ref OptionSelectorMultistate  m_PPQualitySelector;
            protected ref OptionSelectorMultistate  m_SSRQualitySelector;
        #endif
    #endif
    
    #ifdef PLATFORM_PS4
        protected ref ListOptionsAccess         m_FlipModeOption;
    #else
        #ifndef PLATFORM_XBOX
            //Overall
            protected ref ListOptionsAccess         m_OverallQualityOption;
            
            //Screen
            protected ref ListOptionsAccess         m_DisplayModeOption;
            protected ref ListOptionsAccess         m_ResolutionOption;
            protected ref NumericOptionsAccess      m_BrightnessOption;
            protected ref ListOptionsAccess         m_VSyncOption;
            //protected ref ListOptionsAccess           m_ColorDepthOption;
            
            //Scene
            protected ref ListOptionsAccess         m_ObjectDetailOption;
            protected ref ListOptionsAccess         m_TerrainDetailOption;
            protected ref ListOptionsAccess         m_TextureDetailOption;
            protected ref ListOptionsAccess         m_ShadowDetailOption;
            protected ref ListOptionsAccess         m_VisibilityOption;
            protected ref ListOptionsAccess         m_ObjectVisibilityOption;
            
            //Rendering
            protected ref ListOptionsAccess         m_TextureFilteringOption;
            protected ref ListOptionsAccess         m_TerrainSurfaceDetailOption;
            protected ref ListOptionsAccess         m_PPAAOption;
            protected ref ListOptionsAccess         m_HWAAOption;
            protected ref ListOptionsAccess         m_ATOCOption;
            protected ref ListOptionsAccess         m_AOOption;
            protected ref ListOptionsAccess         m_PPQualityOption;  
            protected ref ListOptionsAccess         m_SSRQualityOption; 
        #endif
    #endif
    
    protected ref map<int, ref Param2<string, string>> m_TextMap;
    
    void OptionsMenuVideo( Widget parent, Widget details_root, GameOptions options, OptionsMenu menu )
    {
        m_Root                          = g_Game.GetWorkspace().CreateWidgets( GetLayoutName(), parent );
        
        m_DetailsRoot                   = details_root;
        m_DetailsBodyDefault            = m_DetailsRoot.FindAnyWidget(&quot;settings_details_body&quot;);
        m_DetailsBodyConnectivity       = m_DetailsRoot.FindAnyWidget(&quot;settings_details_body_connectivity&quot;);
        m_DetailsLabel                  = TextWidget.Cast( m_DetailsRoot.FindAnyWidget( &quot;details_label&quot; ) );
        m_DetailsText                   = RichTextWidget.Cast( m_DetailsRoot.FindAnyWidget( &quot;details_content&quot; ) );
        
        SetOptions( options );
        m_Menu                          = menu;
        
        #ifdef PLATFORM_PS4
            m_Root.FindAnyWidget( &quot;vsync_setting_option&quot; ).SetUserID( OptionAccessType.AT_OPTIONS_FLIPMODE );
        #else
            #ifndef PLATFORM_XBOX
                m_Root.FindAnyWidget( &quot;overall_quality_setting_option&quot; ).SetUserID( OptionAccessType.AT_QUALITY_PREFERENCE );
                m_Root.FindAnyWidget( &quot;display_mode_setting_option&quot; ).SetUserID( OptionAccessType.AT_OPTIONS_DISPLAY_MODE );
                m_Root.FindAnyWidget( &quot;resolution_setting_option&quot; ).SetUserID( OptionAccessType.AT_OPTIONS_RESOLUTION );
                m_Root.FindAnyWidget( &quot;brightness_setting_option&quot; ).SetUserID( OptionAccessType.AT_OPTIONS_BRIGHT_SLIDER );
                m_Root.FindAnyWidget( &quot;vsync_setting_option&quot; ).SetUserID( OptionAccessType.AT_VSYNC_VALUE );
                //m_Root.FindAnyWidget( &quot;color_depth_setting_option&quot; ).SetUserID( OptionAccessType.AT_HDR_DETAIL );
                
                //Scene
                m_Root.FindAnyWidget( &quot;object_detail_setting_option&quot; ).SetUserID( OptionAccessType.AT_OBJECTS_DETAIL );
                m_Root.FindAnyWidget( &quot;terrain_detail_setting_option&quot; ).SetUserID( OptionAccessType.AT_OPTIONS_TERRAIN );
                m_Root.FindAnyWidget( &quot;texture_detail_setting_option&quot; ).SetUserID( OptionAccessType.AT_TEXTURE_DETAIL );
                m_Root.FindAnyWidget( &quot;shadow_detail_setting_option&quot; ).SetUserID( OptionAccessType.AT_SHADOW_DETAIL );
                m_Root.FindAnyWidget( &quot;visibility_setting_option&quot; ).SetUserID( OptionAccessType.AT_OPTIONS_VISIBILITY_COMBO );
                m_Root.FindAnyWidget( &quot;object_visibility_setting_option&quot; ).SetUserID( OptionAccessType.AT_OPTIONS_OBJECT_VISIBILITY_COMBO );
                
                //Rendering
                m_Root.FindAnyWidget( &quot;texture_filtering_setting_option&quot; ).SetUserID( OptionAccessType.AT_ANISO_DETAIL );
                m_Root.FindAnyWidget( &quot;terrain_surface_detail_setting_option&quot; ).SetUserID( OptionAccessType.AT_OPTIONS_TERRAIN_SHADER );
                m_Root.FindAnyWidget( &quot;ppaa_setting_option&quot; ).SetUserID( OptionAccessType.AT_OPTIONS_FXAA_VALUE );
                m_Root.FindAnyWidget( &quot;hwaa_setting_option&quot; ).SetUserID( OptionAccessType.AT_FSAA_DETAIL );
                m_Root.FindAnyWidget( &quot;atoc_setting_option&quot; ).SetUserID( OptionAccessType.AT_ATOC_DETAIL );
                m_Root.FindAnyWidget( &quot;ao_setting_option&quot; ).SetUserID( OptionAccessType.AT_AMBIENT_OCCLUSION );
                m_Root.FindAnyWidget( &quot;pp_setting_option&quot; ).SetUserID( OptionAccessType.AT_POSTPROCESS_EFFECTS );
                m_Root.FindAnyWidget( &quot;ssr_setting_option&quot; ).SetUserID( OptionAccessType.AT_WATER_DETAIL );
            #endif
        #endif
        
        FillTextMap();
        InitSelectors();
        RefreshCustom();
        g_Game.BeginOptionsVideo();
        
        float x, y, y2;
        m_Root.FindAnyWidget( &quot;video_settings_scroll&quot; ).GetScreenSize( x, y );
        m_Root.FindAnyWidget( &quot;video_settings_root&quot; ).GetScreenSize( x, y2 );
        int f = ( y2 > y );
        m_Root.FindAnyWidget( &quot;video_settings_scroll&quot; ).SetAlpha( f );
        
        m_Root.SetHandler( this );
