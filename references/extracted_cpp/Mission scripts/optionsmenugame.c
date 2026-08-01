// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/options/optionsmenugame.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionsMenuGame extends ScriptedWidgetEventHandler
{
    private const float HUD_BRIGHTNESS_MIN = -1.0;
    private const float HUD_BRIGHTNESS_MAX = 0.0;
 
    protected Widget                        m_Root;
    
    protected Widget                        m_SettingsRoot;
    protected Widget                        m_DetailsRoot;
    protected Widget                        m_DetailsBodyDefault;
    protected Widget                        m_DetailsBodyConnectivity;
    protected TextWidget                    m_DetailsLabel;
    protected RichTextWidget                m_DetailsText;
    protected ButtonWidget                  m_QuickbarButton;
    
    protected ref NumericOptionsAccess      m_FOVOption;
    protected ref ListOptionsAccess         m_LanguageOption;
    protected ref ListOptionsAccess         m_PauseOption;
    #ifdef PLATFORM_CONSOLE
    protected ref NumericOptionsAccess      m_BrightnessOption;
    protected ref OptionSelectorSlider      m_BrightnessSelector;
    #endif
    
    protected ref OptionSelectorMultistate  m_LanguageSelector;
    protected ref OptionSelectorSlider      m_FOVSelector;
    protected ref OptionSelectorMultistate  m_ShowHUDSelector;
    protected ref OptionSelectorMultistate  m_ShowCrosshairSelector;
    protected ref OptionSelectorMultistate  m_ShowQuickbarSelector;
    protected ref OptionSelectorMultistate  m_ShowHUDVehicleSelector;
    protected ref OptionSelectorMultistate  m_ShowGameSelector;
    protected ref OptionSelectorMultistate  m_ShowAdminSelector;
    protected ref OptionSelectorMultistate  m_ShowPlayerSelector;
    protected ref OptionSelectorMultistate  m_ShowServerInfoSelector;
    protected ref OptionSelectorMultistate  m_BleedingIndication;
    protected ref OptionSelectorMultistate  m_ConnectivityInfo;
    protected ref OptionSelectorSlider      m_HUDBrightnessSelector;
    protected ref OptionSelectorMultistate  m_PauseSelector;
    
    protected GameOptions                   m_Options;
    protected OptionsMenu                   m_Menu;
    
    protected ref map<int, ref Param2<string, string>> m_TextMap;
    
    void OptionsMenuGame(Widget parent, Widget details_root, GameOptions options, OptionsMenu menu)
    {
        m_Root                      = g_Game.GetWorkspace().CreateWidgets(GetLayoutName(), parent);
        
        m_DetailsRoot               = details_root;
        m_DetailsBodyDefault        = m_DetailsRoot.FindAnyWidget(&quot;settings_details_body&quot;);
        m_DetailsBodyConnectivity   = m_DetailsRoot.FindAnyWidget(&quot;settings_details_body_connectivity&quot;);
        m_DetailsLabel              = TextWidget.Cast(m_DetailsRoot.FindAnyWidget(&quot;details_label&quot;));
        m_DetailsText               = RichTextWidget.Cast(m_DetailsRoot.FindAnyWidget(&quot;details_content&quot;));
        
        m_Options                   = options;
        m_Menu                      = menu;
        
        m_FOVOption                 = NumericOptionsAccess.Cast(m_Options.GetOptionByType(OptionAccessType.AT_OPTIONS_FIELD_OF_VIEW));
        m_LanguageOption            = ListOptionsAccess.Cast(m_Options.GetOptionByType(OptionAccessType.AT_OPTIONS_LANGUAGE));
        m_PauseOption               = ListOptionsAccess.Cast(m_Options.GetOptionByType(OptionAccessType.AT_OPTIONS_PAUSE));
        
        m_Root.FindAnyWidget(&quot;fov_setting_option&quot;).SetUserID(OptionAccessType.AT_OPTIONS_FIELD_OF_VIEW);
        m_Root.FindAnyWidget(&quot;hud_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_HUD);
        m_Root.FindAnyWidget(&quot;hud_vehicle_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_HUD_VEHICLE);
        m_Root.FindAnyWidget(&quot;hud_brightness_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_HUD_BRIGHTNESS);
        m_Root.FindAnyWidget(&quot;crosshair_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_CROSSHAIR);
        m_Root.FindAnyWidget(&quot;game_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_GAME_MESSAGES);
        m_Root.FindAnyWidget(&quot;admin_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_ADMIN_MESSAGES);
        m_Root.FindAnyWidget(&quot;player_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_PLAYER_MESSAGES);
        m_Root.FindAnyWidget(&quot;language_setting_option&quot;).SetUserID(OptionAccessType.AT_OPTIONS_LANGUAGE);
        m_Root.FindAnyWidget(&quot;bleeding_indication_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_BLEEDINGINDICATION);
        m_Root.FindAnyWidget(&quot;connectivity_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_CONNECTIVITY_INFO);
        m_Root.FindAnyWidget(&quot;quickbar_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_QUICKBAR);
        
        #ifdef PLATFORM_CONSOLE
        m_QuickbarButton = ButtonWidget.Cast(m_Root.FindAnyWidget(&quot;quickbar_button&quot;));
        m_Root.FindAnyWidget(&quot;brightness_setting_option&quot;).SetUserID(OptionAccessType.AT_OPTIONS_BRIGHT_SLIDER);
        #else
        m_Root.FindAnyWidget(&quot;serverinfo_setting_option&quot;).SetUserID(OptionIDsScript.OPTION_SERVER_INFO);
        m_Root.FindAnyWidget(&quot;pause_setting_option&quot;).SetUserID(OptionAccessType.AT_OPTIONS_PAUSE);
        #endif
        
        FillTextMap();
        
        array<string> opt       = { &quot;#options_controls_disabled&quot;, &quot;#options_controls_enabled&quot; };
        array<string> opt2      = { &quot;#options_controls_enabled&quot;, &quot;#options_controls_disabled&quot; };
        array<string> opt3      = new array<string>;
        for (int i = 0; i < m_LanguageOption.GetItemsCount(); ++i)
        {
            string text;
            m_LanguageOption.GetItemText(i, text);
            opt3.Insert(text);
        }
        
        array<string> opt4      = { &quot;#options_pc_nopause_0&quot;, &quot;#options_pc_nopause_1&quot;, &quot;#options_pc_nopause_2&quot; }; // Pause options menu strings
        
        m_LanguageSelector          = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;language_setting_option&quot;), m_LanguageOption.GetIndex(), this, false, opt3);
        m_FOVSelector               = new OptionSelectorSlider(m_Root.FindAnyWidget(&quot;fov_setting_option&quot;), m_FOVOption.ReadValue(), this, false, m_FOVOption.GetMin(), m_FOVOption.GetMax());
        m_ShowHUDSelector           = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;hud_setting_option&quot;), g_Game.GetProfileOptionBool(EDayZProfilesOptions.HUD), this, false, opt);
        m_HUDBrightnessSelector     = new OptionSelectorSlider(m_Root.FindAnyWidget(&quot;hud_brightness_setting_option&quot;), g_Game.GetProfileOptionFloat(EDayZProfilesOptions.HUD_BRIGHTNESS), this, false, HUD_BRIGHTNESS_MIN, HUD_BRIGHTNESS_MAX);
        m_ShowCrosshairSelector     = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;crosshair_setting_option&quot;), g_Game.GetProfileOptionBool(EDayZProfilesOptions.CROSSHAIR), this, false, opt);
        m_ShowGameSelector          = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;game_setting_option&quot;), g_Game.GetProfileOptionBool(EDayZProfilesOptions.GAME_MESSAGES), this, false, opt2);
        m_ShowAdminSelector         = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;admin_setting_option&quot;), g_Game.GetProfileOptionBool(EDayZProfilesOptions.ADMIN_MESSAGES), this, false, opt2);
        m_ShowPlayerSelector        = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;player_setting_option&quot;), g_Game.GetProfileOptionBool(EDayZProfilesOptions.PLAYER_MESSAGES), this, false, opt2);
        m_BleedingIndication        = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;bleeding_indication_setting_option&quot;), g_Game.GetProfileOptionBool(EDayZProfilesOptions.BLEEDINGINDICATION), this, false, opt);
        m_ConnectivityInfo          = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;connectivity_setting_option&quot;), g_Game.GetProfileOptionBool(EDayZProfilesOptions.CONNECTIVITY_INFO), this, false, opt);
        m_ShowQuickbarSelector      = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;quickbar_setting_option&quot;), g_Game.GetProfileOptionBool(EDayZProfilesOptions.QUICKBAR), this, false, opt);
        m_ShowHUDVehicleSelector    = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;hud_vehicle_setting_option&quot;), g_Game.GetProfileOptionBool(EDayZProfilesOptions.HUD_VEHICLE), this, false, opt);
        
        m_LanguageSelector.m_OptionChanged.Insert(UpdateLanguageOption);
        m_FOVSelector.m_OptionChanged.Insert(UpdateFOVOption);
        m_ShowHUDSelector.m_OptionChanged.Insert(UpdateHUDOption);
        m_HUDBrightnessSelector.m_OptionChanged.Insert(UpdateHUDBrightnessOption);
        m_ShowCrosshairSelector.m_OptionChanged.Insert(UpdateCrosshairOption);
        m_ShowGameSelector.m_OptionChanged.Insert(UpdateGameOption);
        m_ShowAdminSelector.m_OptionChanged.Insert(UpdateAdminOption);
        m_ShowPlayerSelector.m_OptionChanged.Insert(UpdatePlayerOption);
        m_BleedingIndication.m_OptionChanged.Insert(UpdateBleedingIndication);
        m_ConnectivityInfo.m_OptionChanged.Insert(UpdateConnectivityInfo);
        m_ShowQuickbarSelector.m_OptionChanged.Insert(UpdateQuickbarOption);
        m_ShowHUDVehicleSelector.m_OptionChanged.Insert(UpdateHUDVehicleOption);
        
        #ifdef PLATFORM_CONSOLE
        m_BrightnessOption      = NumericOptionsAccess.Cast(m_Options.GetOptionByType(OptionAccessType.AT_OPTIONS_BRIGHT_SLIDER));
        m_BrightnessSelector    = new OptionSelectorSlider(m_Root.FindAnyWidget(&quot;brightness_setting_option&quot;), m_BrightnessOption.ReadValue(), this, false, m_BrightnessOption.GetMin(), m_BrightnessOption.GetMax());
        m_BrightnessSelector.m_OptionChanged.Insert(UpdateBrightnessOption);
        #else
        m_ShowServerInfoSelector    = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;serverinfo_setting_option&quot;), g_Game.GetProfileOptionBool(EDayZProfilesOptions.SERVERINFO_DISPLAY), this, false, opt);
        m_PauseSelector             = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;pause_setting_option&quot;), m_PauseOption.GetIndex(), this, false, opt4);
            
        m_ShowServerInfoSelector.m_OptionChanged.Insert(UpdateServerInfoOption);
        m_PauseSelector.m_OptionChanged.Insert(UpdatePauseOption);
        #endif
        
        float x, y, y2;
        m_Root.FindAnyWidget(&quot;game_settings_scroll&quot;).GetScreenSize(x, y);
        m_Root.FindAnyWidget(&quot;game_settings_root&quot;).GetScreenSize(x, y2);
        int f = (y2 > y);
        m_Root.FindAnyWidget(&quot;game_settings_scroll&quot;).SetAlpha(f);
        
        m_Root.SetHandler(this);
        m_DetailsRoot.Show(false);
        
        InitDependentOptionsVisibility();
