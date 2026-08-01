// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/mainmenu/bannerhandlerbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BannerHandlerBase extends ScriptedWidgetEventHandler
{
    protected Widget m_Root;
    protected Widget m_Parent; //frame
    protected Widget m_BannerBody;
    protected Widget m_BannerWedge;
    protected TextWidget m_BannerText;
    
    void BannerHandlerBase(Widget parent)
    {
        m_Parent = parent;
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/dlc_panels/BannerTemplate.layout&quot;, m_Parent);
        m_Root.SetHandler(this);
        
        m_BannerBody = m_Root.FindAnyWidget(&quot;BadgeBody&quot;);
        m_BannerWedge = m_Root.FindAnyWidget(&quot;BadgeWedge&quot;);
        m_BannerText = TextWidget.Cast(m_Root.FindAnyWidget(&quot;BadgeText&quot;));
        
        m_BannerBody.SetAlpha(1.0);
        m_BannerWedge.SetAlpha(1.0);
