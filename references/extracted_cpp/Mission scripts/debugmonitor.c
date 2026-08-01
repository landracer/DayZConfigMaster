// Source: F:/Games/Dayz/scripts/5_mission/gui/debugmonitor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DebugMonitor
{
    protected bool m_IsUsingKBM;
    
    private Widget m_WidgetRoot;
    private TextWidget m_WindowLabelText;
    
    private TextWidget m_VersionValue;
    private TextWidget m_HealthValue;
    private TextWidget m_BloodValue;
    private TextWidget m_DmgSourceValue;
    private TextWidget m_MapTileValue;
    private TextWidget m_PositionValue;
    private TextWidget m_CopyPositionInfo;
    
    private TextWidget m_FPSValue;
    private TextWidget m_FPSMinValue;
    private TextWidget m_FPSMaxValue;
    private TextWidget m_FPSAvgValue;
    
    private int m_FPSTextDefaultColor;
        
    void DebugMonitor()
    {       
        m_WidgetRoot = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/debug/day_z_debug_monitor.layout&quot;);
        m_WidgetRoot.Show(false);
        
        m_VersionValue = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;VersionValue&quot;));
        m_HealthValue = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;HealthValue&quot;));
        m_BloodValue = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;BloodValue&quot;));
        m_DmgSourceValue = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;DmgSourceValue&quot;));
        m_MapTileValue = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;MapTileValue&quot;));
        m_PositionValue = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;PositionValue&quot;));
        m_CopyPositionInfo = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;CopyPositionInfo&quot;));
        
        m_FPSValue = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;FPSCurrentValue&quot;));
        m_FPSMinValue = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;FPSMinValue&quot;));
        m_FPSMaxValue = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;FPSMaxValue&quot;));
        m_FPSAvgValue = TextWidget.Cast(m_WidgetRoot.FindAnyWidget(&quot;FPSAvgValue&quot;));
        
        m_FPSTextDefaultColor = m_FPSValue.GetColor();
