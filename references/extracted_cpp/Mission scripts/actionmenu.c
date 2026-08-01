// Source: F:/Games/Dayz/scripts/5_mission/gui/actionmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionMenu
{
    private const float FADE_IN_TIME = 0.3;
    private const float FADE_OUT_TIME = 0.3;
    private const float HIDE_MENU_TIME = 5;
 
    private ref WidgetFadeTimer m_FadeTimer;
    private ref array<TextWidget> m_actionItems;
    private TextWidget m_defaultActionWidget;
    private Widget m_actionsPanelWidget;
    private ref Timer m_hide_timer;
    
    private bool m_visible;
 
    void ActionMenu()
    {
        m_actionItems = new array<TextWidget>;
        m_FadeTimer = new WidgetFadeTimer;
        m_hide_timer = new Timer(CALL_CATEGORY_GUI);
        m_visible = false;
