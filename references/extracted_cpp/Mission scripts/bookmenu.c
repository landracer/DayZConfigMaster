// Source: F:/Games/Dayz/scripts/5_mission/gui/bookmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BookMenu extends UIScriptedMenu
{
    protected TextWidget m_author;
    protected TextWidget m_title;
    protected TextWidget m_page;
    protected HtmlWidget m_content;
    protected float m_page_height;
    protected float m_content_total_height;
    protected float m_content_pos;
 
    override Widget Init()
    {
        layoutRoot = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/day_z_book.layout&quot;);
        Class.CastTo(m_content, layoutRoot.FindAnyWidget(&quot;HtmlWidget&quot;));
        Class.CastTo(m_author, layoutRoot.FindAnyWidget(&quot;Author&quot;)); 
        Class.CastTo(m_title, layoutRoot.FindAnyWidget(&quot;Title&quot;));
        Class.CastTo(m_page, layoutRoot.FindAnyWidget(&quot;Page&quot;));
        
        float width;
        m_content.GetScreenSize(width, m_page_height);
        return layoutRoot;
