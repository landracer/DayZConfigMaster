// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/mainmenu/mainmenustats.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MainMenuStats extends ScriptedWidgetEventHandler
{
    protected Widget m_Root;
    
    protected Widget m_TimeSurvived;
    protected TextWidget m_TimeSurvivedValue;
    
    protected Widget m_PlayersKilled;
    protected TextWidget m_PlayersKilledValue;
    
    protected Widget m_InfectedKilled;
    protected TextWidget m_InfectedKilledValue;
    
    protected Widget m_DistanceTraveled;
    protected TextWidget m_DistanceTraveledValue;
    
    protected Widget m_LongRangeShot;
    protected TextWidget m_LongRangeShotValue;
    
    private ref FullTimeData m_TimeSurvivedFull;
    
    void MainMenuStats(Widget root)
    {
        m_Root = root;
        
        m_TimeSurvivedFull = new FullTimeData();
        
        m_TimeSurvived = m_Root.FindAnyWidget(&quot;TimeSurvived&quot;);
        m_TimeSurvivedValue = TextWidget.Cast(m_Root.FindAnyWidget(&quot;TimeSurvivedValue&quot;));
        
        m_PlayersKilled = m_Root.FindAnyWidget(&quot;PlayersKilled&quot;);
        m_PlayersKilledValue = TextWidget.Cast(m_Root.FindAnyWidget(&quot;PlayersKilledValue&quot;));
        
        m_InfectedKilled = m_Root.FindAnyWidget(&quot;InfectedKilled&quot;);
        m_InfectedKilledValue = TextWidget.Cast(m_Root.FindAnyWidget(&quot;InfectedKilledValue&quot;));
        
        m_DistanceTraveled = m_Root.FindAnyWidget(&quot;DistanceTraveled&quot;);
        m_DistanceTraveledValue = TextWidget.Cast(m_Root.FindAnyWidget(&quot;DistanceTraveledValue&quot;));
        
        m_LongRangeShot = m_Root.FindAnyWidget(&quot;LongRangeShot&quot;);
        m_LongRangeShotValue = TextWidget.Cast(m_Root.FindAnyWidget(&quot;LongRangeShotValue&quot;));
