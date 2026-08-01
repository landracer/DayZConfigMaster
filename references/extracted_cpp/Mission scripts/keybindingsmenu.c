// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/keybindings/keybindingsmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class KeybindingsMenu extends UIScriptedMenu
{
    protected TabberUI                          m_Tabber;
    protected ref DropdownPrefab                m_KBDropdown; //DEPRECATED
    protected ref OptionSelectorMultistate      m_PresetSelector;
    protected ref KeybindingsContainer          m_GroupsContainer;
    protected ref array<ref KeybindingsGroup>   m_Tabs; //DEPRECATED
    
    protected TextWidget                        m_Version;
    protected ButtonWidget                      m_Apply;
    protected ButtonWidget                      m_Back;
    protected ButtonWidget                      m_Undo;
    protected ButtonWidget                      m_Defaults;
    protected ButtonWidget                      m_HardReset;
    
    protected int                               m_CurrentSettingKeyIndex = -1;
    protected int                               m_CurrentSettingAlternateKeyIndex = -1;
    protected int                               m_OriginalPresetIndex;
    protected int                               m_TargetPresetIndex;
    protected ref array<int>                    m_SetKeybinds;
    
    const int MODAL_ID_BACK = 1337;
    const int MODAL_ID_DEFAULT = 100;
    const int MODAL_ID_DEFAULT_ALL = 101;
    const int MODAL_ID_PRESET_CHANGE = 200;
    const int MODAL_RESULT_DEFAULT_CURRENT = 0;
    const int MODAL_RESULT_DEFAULT_ALL = 1;
    
    override Widget Init()
    {
        Input input = g_Game.GetInput();
        #ifdef PLATFORM_MSSTORE
        layoutRoot          = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/options/msstore/keybinding_menu.layout&quot;, null);
        #else
        layoutRoot          = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/options/pc/keybinding_menu.layout&quot;, null);
        #endif
 
        m_Version           = TextWidget.Cast(layoutRoot.FindAnyWidget(&quot;version&quot;));
        m_Apply             = ButtonWidget.Cast(layoutRoot.FindAnyWidget(&quot;apply&quot;));
        m_Back              = ButtonWidget.Cast(layoutRoot.FindAnyWidget(&quot;back&quot;));
        m_Undo              = ButtonWidget.Cast(layoutRoot.FindAnyWidget(&quot;undo&quot;));
        m_Defaults          = ButtonWidget.Cast(layoutRoot.FindAnyWidget(&quot;reset&quot;));
        m_HardReset         = ButtonWidget.Cast(layoutRoot.FindAnyWidget(&quot;reset_all&quot;));
        
        layoutRoot.FindAnyWidget(&quot;Tabber&quot;).GetScript(m_Tabber);
        
        string version;
        g_Game.GetVersion(version);
        #ifdef PLATFORM_CONSOLE
        version = &quot;#main_menu_version&quot; + &quot; &quot; + version + &quot; (&quot; + g_Game.GetDatabaseID() + &quot;)&quot;;
        #else
        version = &quot;#main_menu_version&quot; + &quot; &quot; + version;
        #endif
        m_Version.SetText(version);
 
        #ifdef PLATFORM_PS4
        string back = &quot;circle&quot;;
        if (g_Game.GetInput().GetEnterButton() != GamepadButton.A)
            back = &quot;cross&quot;;
 
        ImageWidget toolbar_b = ImageWidget.Cast(layoutRoot.FindAnyWidget(&quot;BackIcon&quot;));
        toolbar_b.LoadImageFile(0, &quot;set:playstation_buttons image:&quot; + back);
        #endif
        
        InitInputSortingMap();
        CreateTabs();
        CreateGroupContainer();
        
        InitPresets(-1, layoutRoot.FindAnyWidget(&quot;group_header&quot;), input);
        m_Tabber.m_OnTabSwitch.Insert(UpdateTabContent);
        m_Tabber.SelectTabControl(0);
        m_Tabber.SelectTabPanel(0);
        g_Game.SetKeyboardHandle(this);
        m_Tabber.RefreshTab(true);
        
        ColorDisabled(m_Apply);
        m_Apply.SetFlags(WidgetFlags.IGNOREPOINTER);
        ColorDisabled(m_Undo);
        m_Undo.SetFlags(WidgetFlags.IGNOREPOINTER);
        ColorWhite(m_Defaults, null);
        m_Defaults.ClearFlags(WidgetFlags.IGNOREPOINTER);
 
        return layoutRoot;
