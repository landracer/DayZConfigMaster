// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/options/optionsmenucontrols.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionsMenuControls extends ScriptedWidgetEventHandler
{
    protected Widget                        m_Root;
    
    protected Widget                        m_SettingsRoot;
    protected Widget                        m_DetailsRoot;
    protected Widget                        m_DetailsBodyDefault;
    protected Widget                        m_DetailsBodyConnectivity;
#ifdef PLATFORM_CONSOLE
    protected bool                          m_MaKOptionAvailable;
    protected Widget                        m_ConsoleControllerSensitivityWidget;
    protected Widget                        m_ConsoleMouseSensitivityWidget;
#endif
    protected TextWidget                    m_DetailsLabel;
    protected RichTextWidget                m_DetailsText;
    protected GridSpacerWidget              m_Keybindings;
    
    protected GameOptions                   m_Options;
    protected OptionsMenu                   m_Menu;
    
    // console options accessors
    protected ref SwitchOptionsAccess       m_KeyboardOption;
    protected ref SwitchOptionsAccess       m_AimHelperOption;
    
    // console options selectors
    protected ref OptionSelectorMultistate  m_KeyboardSelector;
    protected ref OptionSelectorMultistate  m_AimHelperSelector;
 
    // mouse accessors
    protected ref SwitchOptionsAccess       m_Mouse_InvertOption;
    protected ref NumericOptionsAccess      m_Mouse_VSensitivityOption;
    protected ref NumericOptionsAccess      m_Mouse_HSensitivityOption;
    protected ref NumericOptionsAccess      m_Mouse_AimMod_VSensitivityOption;
    protected ref NumericOptionsAccess      m_Mouse_AimMod_HSensitivityOption;
    
    // mouse selectors
    protected ref OptionSelectorMultistate  m_Mouse_InvertSelector;
    protected ref OptionSelectorSlider      m_Mouse_VSensitivitySelector;
    protected ref OptionSelectorSlider      m_Mouse_HSensitivitySelector;
    protected ref OptionSelectorSlider      m_Mouse_AimMod_VSensitivitySelector;
    protected ref OptionSelectorSlider      m_Mouse_AimMod_HSensitivitySelector;
 
    // gamepad/controller accessors
    protected ref NumericOptionsAccess      m_ControllerLS_VSensitivityOption;
    protected ref NumericOptionsAccess      m_ControllerLS_HSensitivityOption;
    protected ref NumericOptionsAccess      m_ControllerLS_VehicleMod_HSensitivityOption;
    protected ref SwitchOptionsAccess       m_ControllerRS_InvertOption;
    protected ref NumericOptionsAccess      m_ControllerRS_VSensitivityOption;
    protected ref NumericOptionsAccess      m_ControllerRS_HSensitivityOption;
    protected ref NumericOptionsAccess      m_ControllerRS_CurvatureOption;
    protected ref NumericOptionsAccess      m_ControllerRS_AimMod_VSensitivityOption;
    protected ref NumericOptionsAccess      m_ControllerRS_AimMod_HSensitivityOption;
    protected ref NumericOptionsAccess      m_ControllerRS_AimMod_CurvatureOption;
    protected ref NumericOptionsAccess      m_ControllerLS_DeadZoneOption;
    protected ref NumericOptionsAccess      m_ControllerRS_DeadZoneOption;
    
    // gamepad/controller selectors
    protected ref OptionSelectorSlider      m_ControllerLS_VSensitivitySelector;
    protected ref OptionSelectorSlider      m_ControllerLS_HSensitivitySelector;
    protected ref OptionSelectorSlider      m_ControllerLS_VehicleMod_HSensitivitySelector;
    protected ref OptionSelectorMultistate  m_ControllerRS_InvertSelector;
    protected ref OptionSelectorSlider      m_ControllerRS_VSensitivitySelector;
    protected ref OptionSelectorSlider      m_ControllerRS_HSensitivitySelector;
    protected ref OptionSelectorSlider      m_ControllerRS_CurvatureSelector;
    protected ref OptionSelectorSlider      m_ControllerRS_AimMod_VSensitivitySelector;
    protected ref OptionSelectorSlider      m_ControllerRS_AimMod_HSensitivitySelector;
    protected ref OptionSelectorSlider      m_ControllerRS_AimMod_CurvatureSelector;
    protected ref OptionSelectorSlider      m_ControllerLS_DeadZoneSelector;
    protected ref OptionSelectorSlider      m_ControllerRS_DeadZoneSelector;    
    
    protected ref map<int, ref Param2<string, string>> m_TextMap;
    
    static const float SLIDER_STEP = 0.01;
    
    protected OptionSelectorBase m_ActiveOption;
    
    void OptionsMenuControls( Widget parent, Widget details_root, GameOptions options, OptionsMenu menu )
    {
        array<string> opt                           = { &quot;#options_controls_disabled&quot;, &quot;#options_controls_enabled&quot; };
 
        m_Root                                      = g_Game.GetWorkspace().CreateWidgets( GetLayoutName(), parent );
        m_Options                                   = options;
        m_Menu                                      = menu;
        
        m_DetailsRoot                               = details_root;
        m_DetailsBodyDefault                        = m_DetailsRoot.FindAnyWidget(&quot;settings_details_body&quot;);
        m_DetailsBodyConnectivity                   = m_DetailsRoot.FindAnyWidget(&quot;settings_details_body_connectivity&quot;);
        m_DetailsLabel                              = TextWidget.Cast( m_DetailsRoot.FindAnyWidget( &quot;details_label&quot; ) );
        m_DetailsText                               = RichTextWidget.Cast( m_DetailsRoot.FindAnyWidget( &quot;details_content&quot; ) );
        m_Keybindings                               = GridSpacerWidget.Cast( m_Root.FindAnyWidget( &quot;keyboard_settings_content&quot; ) );
        if( m_Keybindings )
            m_Keybindings.SetUserID( 777 );
        
        SetOptions( options );
        
        // mouse (PC & consoles)
        Widget mouse_InvertSettingOption = m_Root.FindAnyWidget( &quot;mouse_invert_setting_option&quot; );
        Widget mouse_VSensitivitySettingOption = m_Root.FindAnyWidget( &quot;mouse_vsensitivity_setting_option&quot; );
        Widget mouse_HSensitivitySettingOption = m_Root.FindAnyWidget( &quot;mouse_hsensitivity_setting_option&quot; );
        Widget mouse_AimMod_VSensitivitySettingOption = m_Root.FindAnyWidget( &quot;mouse_aimmod_vsensitivity_setting_option&quot; );
        Widget mouse_AimMod_HSensitivitySettingOption = m_Root.FindAnyWidget( &quot;mouse_aimmod_hsensitivity_setting_option&quot; );
        
        mouse_InvertSettingOption.SetUserID( OptionAccessType.AT_OPTIONS_MOUSE_YAXIS_INVERTED );
        mouse_VSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_MOUSE_YAXIS );
        mouse_HSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_MOUSE_XAXIS );
        mouse_AimMod_VSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_MOUSE_YAXIS_AIM_MOD );
        mouse_AimMod_HSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_MOUSE_XAXIS_AIM_MOD );
 
        m_Mouse_InvertSelector = new OptionSelectorMultistate(mouse_InvertSettingOption, m_Mouse_InvertOption.GetIndex(), this, false, opt );
        
        m_Mouse_VSensitivitySelector = new OptionSelectorSlider(mouse_VSensitivitySettingOption, m_Mouse_VSensitivityOption.ReadValue(), this, false, m_Mouse_VSensitivityOption.GetMin(), m_Mouse_VSensitivityOption.GetMax(), true);
        m_Mouse_VSensitivitySelector.SetStep(SLIDER_STEP);
 
        m_Mouse_HSensitivitySelector = new OptionSelectorSlider(mouse_HSensitivitySettingOption, m_Mouse_HSensitivityOption.ReadValue(), this, false, m_Mouse_HSensitivityOption.GetMin(), m_Mouse_HSensitivityOption.GetMax(), true);
        m_Mouse_HSensitivitySelector.SetStep(SLIDER_STEP);
    
        m_Mouse_AimMod_VSensitivitySelector = new OptionSelectorSlider(mouse_AimMod_VSensitivitySettingOption, m_Mouse_AimMod_VSensitivityOption.ReadValue(), this, false, m_Mouse_AimMod_VSensitivityOption.GetMin(), m_Mouse_AimMod_VSensitivityOption.GetMax(), true);
        m_Mouse_AimMod_VSensitivitySelector.SetStep(SLIDER_STEP);
        
        m_Mouse_AimMod_HSensitivitySelector = new OptionSelectorSlider(mouse_AimMod_HSensitivitySettingOption, m_Mouse_AimMod_HSensitivityOption.ReadValue(), this, false, m_Mouse_AimMod_HSensitivityOption.GetMin(), m_Mouse_AimMod_HSensitivityOption.GetMax(), true);
        m_Mouse_AimMod_HSensitivitySelector.SetStep(SLIDER_STEP);
 
        m_Mouse_InvertSelector.m_OptionChanged.Insert( UpdateMouseInvertView );
        m_Mouse_VSensitivitySelector.m_OptionChanged.Insert( UpdateMouseVSensitivity );
        m_Mouse_HSensitivitySelector.m_OptionChanged.Insert( UpdateMouseHSensitivity );
        m_Mouse_AimMod_VSensitivitySelector.m_OptionChanged.Insert( UpdateMouseAimModVSensitivity );
        m_Mouse_AimMod_HSensitivitySelector.m_OptionChanged.Insert( UpdateMouseAimModHSensitivity );
 
        // controller (consoles only)
        #ifdef PLATFORM_CONSOLE
            m_MaKOptionAvailable = g_Game.GetGameState() != DayZGameState.IN_GAME || g_Game.GetWorld().IsMouseAndKeyboardEnabledOnServer();
            m_ConsoleControllerSensitivityWidget = m_Root.FindAnyWidget( &quot;controller_settings_root&quot; );
            m_ConsoleMouseSensitivityWidget = m_Root.FindAnyWidget( &quot;mouse_settings_root&quot; );
        
            Widget keyboardSettingOption = m_Root.FindAnyWidget( &quot;keyboard_setting_option&quot; );
            Widget aimHelperSettingOption = m_Root.FindAnyWidget( &quot;aimhelper_setting_option&quot; );
        
            Widget controllerLS_VSensitivitySettingOption =  m_Root.FindAnyWidget(&quot;controller_ls_vsensitivity_setting_option&quot;);
            Widget controllerLS_HSensitivitySettingOption =  m_Root.FindAnyWidget(&quot;controller_ls_hsensitivity_setting_option&quot;);
            Widget controllerLS_VehicleMod_HSensitivitySettingOption =  m_Root.FindAnyWidget(&quot;controller_ls_vehicle_sensitivity_setting_option&quot;);
            Widget controllerRS_InvertSettingOption =  m_Root.FindAnyWidget(&quot;controller_rs_invert_setting_option&quot;);
            Widget controllerRS_VSensitivitySettingOption =  m_Root.FindAnyWidget(&quot;controller_rs_vsensitivity_setting_option&quot;);
            Widget controllerRS_HSensitivitySettingOption =  m_Root.FindAnyWidget(&quot;controller_rs_hsensitivity_setting_option&quot;);
            Widget controllerRS_CurvatureSettingOption =  m_Root.FindAnyWidget(&quot;controller_rs_curvature_setting_option&quot;);
            Widget controllerRS_AimMod_VSensitivitySettingOption =  m_Root.FindAnyWidget(&quot;controller_rs_aimmod_vsensitivity_setting_option&quot;);
            Widget controllerRS_AimMod_HSensitivitySettingOption =  m_Root.FindAnyWidget(&quot;controller_rs_aimmod_hsensitivity_setting_option&quot;);
            Widget controllerRS_AimMod_CurvatureSettingOption =  m_Root.FindAnyWidget(&quot;controller_rs_aimmod_curvature_setting_option&quot;);
            Widget controllerLS_DeadZoneOption =  m_Root.FindAnyWidget(&quot;controller_ls_deadzone_setting_option&quot;);
            Widget controllerRS_DeadZoneOption =  m_Root.FindAnyWidget(&quot;controller_rs_deadzone_setting_option&quot;);
        
            keyboardSettingOption.SetUserID( OptionAccessType.AT_OPTIONS_MOUSE_AND_KEYBOARD );
            aimHelperSettingOption.SetUserID( OptionAccessType.AT_OPTIONS_AIM_HELPER );
        
            controllerLS_VSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_LS_YAXIS );
            controllerLS_HSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_LS_XAXIS );
            controllerLS_VehicleMod_HSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_LS_XAXIS_VEHICLE_MOD );
            controllerRS_InvertSettingOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_RS_YAXIS_INVERTED );
            controllerRS_VSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_RS_YAXIS );
            controllerRS_HSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_RS_XAXIS );
            controllerRS_CurvatureSettingOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_RS_CURVATURE );
            controllerRS_AimMod_VSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_RS_YAXIS_AIM_MOD );
            controllerRS_AimMod_HSensitivitySettingOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_RS_XAXIS_AIM_MOD );
            controllerRS_AimMod_CurvatureSettingOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_RS_CURVATURE_AIM_MOD );
            controllerLS_DeadZoneOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_LS_DEADZONE );
            controllerRS_DeadZoneOption.SetUserID( OptionAccessType.AT_OPTIONS_CONTROLLER_RS_DEADZONE );
        
            m_KeyboardSelector = new OptionSelectorMultistate( keyboardSettingOption, m_KeyboardOption.GetIndex(), this, !m_MaKOptionAvailable, opt );
            m_AimHelperSelector = new OptionSelectorMultistate( aimHelperSettingOption, m_AimHelperOption.GetIndex(), this, false, opt );
        
            m_ControllerLS_VSensitivitySelector = new OptionSelectorSlider(controllerLS_VSensitivitySettingOption, m_ControllerLS_VSensitivityOption.ReadValue(), this, false, m_ControllerLS_VSensitivityOption.GetMin(), m_ControllerLS_VSensitivityOption.GetMax(), true);
            m_ControllerLS_VSensitivitySelector.SetStep(SLIDER_STEP);
            
            m_ControllerLS_HSensitivitySelector = new OptionSelectorSlider(controllerLS_HSensitivitySettingOption, m_ControllerLS_HSensitivityOption.ReadValue(), this, false, m_ControllerLS_HSensitivityOption.GetMin(), m_ControllerLS_HSensitivityOption.GetMax(), true);
            m_ControllerLS_HSensitivitySelector.SetStep(SLIDER_STEP);
            
            m_ControllerLS_VehicleMod_HSensitivitySelector = new OptionSelectorSlider(controllerLS_VehicleMod_HSensitivitySettingOption, m_ControllerLS_VehicleMod_HSensitivityOption.ReadValue(), this, false, m_ControllerLS_VehicleMod_HSensitivityOption.GetMin(), m_ControllerLS_VehicleMod_HSensitivityOption.GetMax(), true);
            m_ControllerLS_VehicleMod_HSensitivitySelector.SetStep(SLIDER_STEP);
            
            m_ControllerRS_InvertSelector = new OptionSelectorMultistate(controllerRS_InvertSettingOption, m_ControllerRS_InvertOption.GetIndex(), this, false, opt);
            
            m_ControllerRS_VSensitivitySelector = new OptionSelectorSlider(controllerRS_VSensitivitySettingOption, m_ControllerRS_VSensitivityOption.ReadValue(), this, false, m_ControllerRS_VSensitivityOption.GetMin(), m_ControllerRS_VSensitivityOption.GetMax(), true);
            m_ControllerRS_VSensitivitySelector.SetStep(SLIDER_STEP);
            
            m_ControllerRS_HSensitivitySelector = new OptionSelectorSlider(controllerRS_HSensitivitySettingOption, m_ControllerRS_HSensitivityOption.ReadValue(), this, false, m_ControllerRS_HSensitivityOption.GetMin(), m_ControllerRS_HSensitivityOption.GetMax(), true);
            m_ControllerRS_HSensitivitySelector.SetStep(SLIDER_STEP);
            
            m_ControllerRS_CurvatureSelector = new OptionSelectorSlider(controllerRS_CurvatureSettingOption, m_ControllerRS_CurvatureOption.ReadValue(), this, false, m_ControllerRS_CurvatureOption.GetMin(), m_ControllerRS_CurvatureOption.GetMax(), true);
            m_ControllerRS_CurvatureSelector.SetStep(SLIDER_STEP);
            
            m_ControllerRS_AimMod_VSensitivitySelector = new OptionSelectorSlider(controllerRS_AimMod_VSensitivitySettingOption, m_ControllerRS_AimMod_VSensitivityOption.ReadValue(), this, false, m_ControllerRS_AimMod_VSensitivityOption.GetMin(), m_ControllerRS_AimMod_VSensitivityOption.GetMax(), true);
            m_ControllerRS_AimMod_VSensitivitySelector.SetStep(SLIDER_STEP);
            
            m_ControllerRS_AimMod_HSensitivitySelector = new OptionSelectorSlider(controllerRS_AimMod_HSensitivitySettingOption, m_ControllerRS_AimMod_HSensitivityOption.ReadValue(), this, false, m_ControllerRS_AimMod_HSensitivityOption.GetMin(), m_ControllerRS_AimMod_HSensitivityOption.GetMax(), true);
            m_ControllerRS_AimMod_HSensitivitySelector.SetStep(SLIDER_STEP);
            
            m_ControllerRS_AimMod_CurvatureSelector = new OptionSelectorSlider(controllerRS_AimMod_CurvatureSettingOption, m_ControllerRS_AimMod_CurvatureOption.ReadValue(), this, false, m_ControllerRS_AimMod_CurvatureOption.GetMin(), m_ControllerRS_AimMod_CurvatureOption.GetMax(), true);
            m_ControllerRS_AimMod_CurvatureSelector.SetStep(SLIDER_STEP);
            
            m_ControllerLS_DeadZoneSelector = new OptionSelectorSlider(controllerLS_DeadZoneOption, m_ControllerLS_DeadZoneOption.ReadValue(), this, false, m_ControllerLS_DeadZoneOption.GetMin(), m_ControllerLS_DeadZoneOption.GetMax(), true);
            m_ControllerLS_DeadZoneSelector.SetStep(SLIDER_STEP);
            
            m_ControllerRS_DeadZoneSelector = new OptionSelectorSlider(controllerRS_DeadZoneOption, m_ControllerRS_DeadZoneOption.ReadValue(), this, false, m_ControllerRS_DeadZoneOption.GetMin(), m_ControllerRS_DeadZoneOption.GetMax(), true);
            m_ControllerRS_DeadZoneSelector.SetStep(SLIDER_STEP);
        
            m_KeyboardSelector.m_OptionChanged.Insert( UpdateKeyboard );
            m_AimHelperSelector.m_OptionChanged.Insert( UpdateAimHelper );
        
            m_ControllerLS_VSensitivitySelector.m_OptionChanged.Insert( UpdateControllerLS_VSensitivity );
            m_ControllerLS_HSensitivitySelector.m_OptionChanged.Insert( UpdateControllerLS_HSensitivity );
            m_ControllerLS_VehicleMod_HSensitivitySelector.m_OptionChanged.Insert( UpdateControllerLS_VehicleMod_HSensitivity );
            m_ControllerRS_InvertSelector.m_OptionChanged.Insert( UpdateControllerRS_InvertView );
            m_ControllerRS_VSensitivitySelector.m_OptionChanged.Insert( UpdateControllerRS_VSensitivity );
            m_ControllerRS_HSensitivitySelector.m_OptionChanged.Insert( UpdateControllerRS_HSensitivity );
            m_ControllerRS_CurvatureSelector.m_OptionChanged.Insert( UpdateControllerRS_Curvature );
            m_ControllerRS_AimMod_VSensitivitySelector.m_OptionChanged.Insert( UpdateControllerRS_AimMod_VSensitivity );
            m_ControllerRS_AimMod_HSensitivitySelector.m_OptionChanged.Insert( UpdateControllerRS_AimMod_HSensitivity );
            m_ControllerRS_AimMod_CurvatureSelector.m_OptionChanged.Insert( UpdateControllerRS_AimMod_Curvature );
            m_ControllerLS_DeadZoneSelector.m_OptionChanged.Insert( UpdateControllerLS_Deadzone );
            m_ControllerRS_DeadZoneSelector.m_OptionChanged.Insert( UpdateControllerRS_Deadzone );
        
            ShowConsoleSensitivityOptions(m_KeyboardOption.GetIndex());
        
            bool MaKState = m_KeyboardSelector.IsEnabled() && g_Game.GetInput().IsEnabledMouseAndKeyboardEvenOnServer();
            g_Game.GetCallQueue(CALL_CATEGORY_GUI).Call(m_Menu.ToggleDependentOptions,EDependentOptions.MOUSEANDKEYBOARD_QUICKBAR,MaKState);
        #endif
                
        FillTextMap();
        
        float x, y, y2;
        m_Root.FindAnyWidget( &quot;controls_settings_scroll&quot; ).GetScreenSize( x, y );
        m_Root.FindAnyWidget( &quot;controls_settings_root&quot; ).GetScreenSize( x, y2 );
        int f = ( y2 > y );
        m_Root.FindAnyWidget( &quot;controls_settings_scroll&quot; ).SetAlpha( f );
        
        m_Root.SetHandler( this );
