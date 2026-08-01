// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/options/optionsmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OptionsMenu extends UIScriptedMenu
{
    const int MODAL_ID_DEFAULT = 100;
    const int DIALOG_TAB_OFFSET = 1400;
 
    protected TabberUI                  m_Tabber;
    protected ref OptionsMenuGame       m_GameTab;
    protected ref OptionsMenuSounds     m_SoundsTab;
    protected ref OptionsMenuVideo      m_VideoTab;
    protected ref OptionsMenuControls   m_ControlsTab;
    
    protected ref GameOptions           m_Options;
    
    protected ButtonWidget              m_Apply;
    protected ButtonWidget              m_Back;
    protected ButtonWidget              m_Reset; //undo
    protected ButtonWidget              m_Defaults; //defaults
    
    protected Widget                    m_Details;
    protected TextWidget                m_Version;
    
    protected int                       m_ActiveTabIdx = 0;
    protected bool                      m_ModalLock;
    protected bool                      m_CanApplyOrReset;
    protected bool                      m_CanToggle;
    
    #ifdef PLATFORM_MSSTORE
    protected ButtonWidget                  m_GamepadControls;
    protected ButtonWidget                  m_KeyboardBindings;
    #endif
    
    void OptionsMenu()
    {
        
