// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsole/scriptconsoleuniversalinfodialog.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsoleUniversalInfoDialog extends UIScriptedMenu
{
    private const int EDITBOX_TEXT_SIZE = 16;
    
    private TextWidget m_Label;
    private ScrollWidget m_ContentScroll;
    private MultilineEditBoxWidget m_Content;
    private ButtonWidget m_OKButton;
    
    void ScriptConsoleUniversalInfoDialog();
    void ~ScriptConsoleUniversalInfoDialog();
 
    override Widget Init()
    {
        layoutRoot = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/script_console/script_console_universal_info_dialog.layout&quot;);
        m_Label = TextWidget.Cast(layoutRoot.FindAnyWidget(&quot;Label&quot;));
        m_ContentScroll = ScrollWidget.Cast(layoutRoot.FindAnyWidget(&quot;ContentScroll&quot;));
        m_ContentScroll.VScrollToPos(0);
        m_Content = MultilineEditBoxWidget.Cast(layoutRoot.FindAnyWidget(&quot;Content&quot;));
        m_OKButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget(&quot;MessageText&quot;));
 
        return layoutRoot;
