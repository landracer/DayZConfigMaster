// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/consoletoolbarhandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class ConsoleToolbarWidgetHandlerBase : ScriptedWidgetEventHandler
{
    Widget m_ToolbarWidget; //&#39;toolbar_bg&#39;
    RichTextWidget m_ToolbarText;
    
    void OnWidgetScriptInit(Widget w)
    {
        m_ToolbarWidget = w;
        m_ToolbarWidget.SetHandler(this);
        
        m_ToolbarText = RichTextWidget.Cast(m_ToolbarWidget.FindAnyWidget(&quot;ContextToolbarText&quot;));
        #ifdef PLATFORM_CONSOLE
        if (g_Game.GetMission())
        {
            g_Game.GetMission().GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
        }
        #endif
        UpdateControlsElements();
