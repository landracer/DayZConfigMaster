// Source: F:/Games/Dayz/scripts/5_mission/gui/watermark.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Watermark
{
    protected Widget m_Root;
    protected ImageWidget m_WatermarkImage;
    protected TextWidget m_WatermarkLabel;
    protected TextWidget m_WatermarkBuild;
 
    void Watermark(Widget parent = null)
    {
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/day_z_hud_watermark.layout&quot;, parent);
        
        m_WatermarkImage = ImageWidget.Cast(m_Root.FindAnyWidget(&quot;WatermarkLogo&quot;));
        m_WatermarkLabel = TextWidget.Cast(m_Root.FindAnyWidget(&quot;WatermarkLabel&quot;));
        m_WatermarkBuild = TextWidget.Cast(m_Root.FindAnyWidget(&quot;WatermarkBuild&quot;));
        m_WatermarkImage.LoadImageFile(0, &quot;set:dayz_gui image:DayZLogo&quot;);
        m_WatermarkLabel.SetText(&quot;EXPERIMENTAL BUILD&quot;);
 
        string buildVersion;
        g_Game.GetVersion(buildVersion);
        m_WatermarkBuild.SetText(buildVersion);
