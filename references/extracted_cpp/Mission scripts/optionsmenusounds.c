// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/options/optionsmenusounds.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionsMenuSounds extends ScriptedWidgetEventHandler
{
    protected Widget                        m_Root;
    
    protected Widget                        m_SettingsRoot;
    protected Widget                        m_DetailsRoot;
    protected Widget                        m_DetailsBodyDefault;
    protected Widget                        m_DetailsBodyConnectivity;
    protected TextWidget                    m_DetailsLabel;
    protected RichTextWidget                m_DetailsText;
    
    protected ref NumericOptionsAccess      m_MasterOption;
    protected ref NumericOptionsAccess      m_EffectsOption;
    protected ref NumericOptionsAccess      m_VOIPOption;
    protected ref NumericOptionsAccess      m_VOIPThresholdOption;
    protected ref NumericOptionsAccess      m_MusicOption;
    protected ref ListOptionsAccess         m_InputModeOption;
    
    protected ref OptionSelectorSlider      m_MasterSelector;
    protected ref OptionSelectorSlider      m_EffectsSelector;
    protected ref OptionSelectorSlider      m_VOIPSelector;
    protected ref OptionSelectorLevelMarker m_VOIPThresholdSelector;
    protected ref OptionSelectorSlider      m_MusicSelector;
    protected ref OptionSelectorMultistate  m_InputModeSelector;
    protected ref OptionSelectorMultistate  m_AmbientMusicSelector;
    
    protected ref Timer                     m_AudioLevelTimer;
    protected GameOptions                   m_Options;
    protected OptionsMenu                   m_Menu;
    protected MissionGameplay               m_MissionGameplay;
    protected VONManagerBase                m_VonManager;
    
    protected ref map<int, ref Param2<string, string>> m_TextMap;
    
    private bool                            m_WasMicCapturing;
    
    void OptionsMenuSounds(Widget parent, Widget details_root, GameOptions options, OptionsMenu menu)
    {
        m_Root                  = g_Game.GetWorkspace().CreateWidgets(GetLayoutName(), parent);
        m_DetailsRoot           = details_root;
        m_DetailsBodyDefault        = m_DetailsRoot.FindAnyWidget(&quot;settings_details_body&quot;);
        m_DetailsBodyConnectivity   = m_DetailsRoot.FindAnyWidget(&quot;settings_details_body_connectivity&quot;);
        m_DetailsLabel          = TextWidget.Cast( m_DetailsRoot.FindAnyWidget(&quot;details_label&quot;));
        m_DetailsText           = RichTextWidget.Cast( m_DetailsRoot.FindAnyWidget(&quot;details_content&quot;)); 
        m_Options               = options;
        m_Menu                  = menu;
        
        m_MasterOption          = NumericOptionsAccess.Cast(m_Options.GetOptionByType(OptionAccessType.AT_OPTIONS_MASTER_VOLUME));
        m_EffectsOption         = NumericOptionsAccess.Cast(m_Options.GetOptionByType(OptionAccessType.AT_OPTIONS_EFFECTS_SLIDER));
        m_MusicOption           = NumericOptionsAccess.Cast(m_Options.GetOptionByType(OptionAccessType.AT_OPTIONS_MUSIC_SLIDER));
        m_VOIPOption            = NumericOptionsAccess.Cast(m_Options.GetOptionByType(OptionAccessType.AT_OPTIONS_VON_SLIDER));
        m_VOIPThresholdOption   = NumericOptionsAccess.Cast(m_Options.GetOptionByType(OptionAccessType.AT_OPTIONS_VON_THRESHOLD_SLIDER));
        m_InputModeOption       = ListOptionsAccess.Cast(m_Options.GetOptionByType(OptionAccessType.AT_OPTIONS_VON_INPUT_MODE));
        
        m_MissionGameplay       = MissionGameplay.Cast(g_Game.GetMission());
        m_VonManager            = VONManager.GetInstance();
        m_AudioLevelTimer = new Timer();
        m_AudioLevelTimer.Run(0.1, this, &quot;UpdateAudioLevel&quot;, null, true); 
        
        m_Root.FindAnyWidget( &quot;master_setting_option&quot; ).SetUserID(OptionAccessType.AT_OPTIONS_MASTER_VOLUME);
        m_Root.FindAnyWidget( &quot;effects_setting_option&quot; ).SetUserID(OptionAccessType.AT_OPTIONS_EFFECTS_SLIDER);
        m_Root.FindAnyWidget( &quot;music_setting_option&quot; ).SetUserID(OptionAccessType.AT_OPTIONS_MUSIC_SLIDER);
        m_Root.FindAnyWidget( &quot;voip_output_setting_option&quot; ).SetUserID(OptionAccessType.AT_OPTIONS_VON_SLIDER);
        m_Root.FindAnyWidget( &quot;voip_threshold_setting_option&quot; ).SetUserID(OptionAccessType.AT_OPTIONS_VON_THRESHOLD_SLIDER);
        m_Root.FindAnyWidget( &quot;voip_selection_setting_option&quot; ).SetUserID(OptionAccessType.AT_OPTIONS_VON_INPUT_MODE);
        m_Root.FindAnyWidget( &quot;ambient_music_mode_option&quot; ).SetUserID(OptionIDsScript.OPTION_AMBIENT_MUSIC_MODE);
        
        FillTextMap();
        
        array<string> inputModeValues = {
            &quot;#STR_Controls_PushToTalk&quot;,
            &quot;#STR_USRACT_UAVOICEOVERNETTOGGLE&quot;,
        };
        array<string> ambientMusicModeValues = {
            &quot;#STR_Ambient_Music_Enabled&quot;,
            &quot;#STR_Ambient_Music_Menu_Only&quot;,
        };
        
        m_MasterSelector            = new OptionSelectorSlider(m_Root.FindAnyWidget(&quot;master_setting_option&quot; ), m_MasterOption.ReadValue(), this, false, m_MasterOption.GetMin(), m_MasterOption.GetMax());
        m_EffectsSelector           = new OptionSelectorSlider(m_Root.FindAnyWidget(&quot;effects_setting_option&quot; ), m_EffectsOption.ReadValue(), this, false, m_EffectsOption.GetMin(), m_EffectsOption.GetMax());
        m_VOIPSelector              = new OptionSelectorSlider(m_Root.FindAnyWidget(&quot;voip_output_setting_option&quot; ), m_VOIPOption.ReadValue(), this, false, m_VOIPOption.GetMin(), m_VOIPOption.GetMax());
        m_VOIPThresholdSelector     = new OptionSelectorLevelMarker(m_Root.FindAnyWidget(&quot;voip_threshold_setting_option&quot; ), m_VOIPThresholdOption.ReadValue(), this, false, m_VOIPThresholdOption.GetMin(), m_VOIPThresholdOption.GetMax());
        m_MusicSelector             = new OptionSelectorSlider(m_Root.FindAnyWidget(&quot;music_setting_option&quot; ), m_MusicOption.ReadValue(), this, false, m_MusicOption.GetMin(), m_MusicOption.GetMax());
        m_InputModeSelector         = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;voip_selection_setting_option&quot; ), m_InputModeOption.GetIndex(), this, false, inputModeValues);
        m_AmbientMusicSelector      = new OptionSelectorMultistate(m_Root.FindAnyWidget(&quot;ambient_music_mode_option&quot; ), g_Game.GetProfileOptionInt(EDayZProfilesOptions.AMBIENT_MUSIC_MODE), this, false, ambientMusicModeValues);
        
        m_MasterSelector.m_OptionChanged.Insert(UpdateMaster);
        m_EffectsSelector.m_OptionChanged.Insert(UpdateEffects);
        m_VOIPSelector.m_OptionChanged.Insert(UpdateVOIP);
        m_VOIPThresholdSelector.m_OptionChanged.Insert(UpdateVOIPThreshold);
        m_MusicSelector.m_OptionChanged.Insert(UpdateMusic);
        m_AmbientMusicSelector.m_OptionChanged.Insert(UpdateAmbientSoundModeOption);
        m_InputModeSelector.m_OptionChanged.Insert(UpdateInputMode);
        
        if (m_MissionGameplay)
        {
            // event to monitor when options get reverted directly from C++
            m_VOIPThresholdOption.GetEvents().Event_OnRevert.Insert(m_VonManager.OnVOIPThresholdChanged);
            m_VonManager.m_OnVonStateEvent.Insert(OnVonStateEvent);
            m_VonManager.m_OnPartyChatChangedEvent.Insert(OnPartyChatChangedEvent);
            m_VOIPThresholdSelector.m_OptionChanged.Insert(m_VonManager.OnVOIPThresholdChanged);
        }
        
        float x, y, y2;
        m_Root.FindAnyWidget(&quot;sound_settings_scroll&quot;).GetScreenSize(x, y);
        m_Root.FindAnyWidget(&quot;sound_settings_root&quot;).GetScreenSize(x, y2);
        int f = (y2 > y);
        m_Root.FindAnyWidget(&quot;sound_settings_scroll&quot;).SetAlpha(f);
        
        m_Root.SetHandler(this);
        
        m_WasMicCapturing = g_Game.IsMicCapturing();
 
        // do not enable mic capture if user in party chat
        if (!g_Game.IsInPartyChat())
        {
            g_Game.EnableMicCapture(true);
        }
