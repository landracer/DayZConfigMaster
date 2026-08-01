// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/areas/leftarea.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LeftArea: Container
{
    protected Widget                    m_UpIcon;
    protected Widget                    m_DownIcon;
    protected Widget                    m_ContentParent;
    protected ref VicinityContainer     m_VicinityContainer;
    protected Widget                    m_SlotsHeader;
    protected Widget                    m_SlotsContent;
    protected ScrollWidget              m_SlotsScrollWidget;
    protected ScrollWidget              m_ScrollWidget;
    protected ref SizeToChild           m_ContentResize;
    protected bool                      m_ShouldChangeSize = true;
    protected bool                      m_IsProcessing = false; // Prevents refreshing every time a child is added while it is still processing
    
    void LeftArea(LayoutHolder parent )
    {
        m_MainWidget.Show(true, false);
        
        m_ContentParent = m_MainWidget.FindAnyWidget(&quot;ContentParent&quot;);
        m_ContentParent.GetScript(m_ContentResize);
        
        #ifndef PLATFORM_CONSOLE
        m_SlotsHeader   = m_MainWidget.FindAnyWidget(&quot;SlotsHeader&quot;);
        m_SlotsContent  = m_MainWidget.FindAnyWidget(&quot;SlotsContent&quot;);
        m_SlotsScrollWidget = ScrollWidget.Cast(m_MainWidget.FindAnyWidget(&quot;ScrollerSlotsContent&quot;));
        #endif
        
        m_ScrollWidget  = ScrollWidget.Cast(m_MainWidget.FindAnyWidget(&quot;Scroller&quot;));
        m_MainWidget    = m_MainWidget.FindAnyWidget(&quot;Content&quot;);
        
        m_UpIcon        = m_RootWidget.FindAnyWidget(&quot;Up&quot;);
        m_DownIcon      = m_RootWidget.FindAnyWidget(&quot;Down&quot;);
        
        m_VicinityContainer = new VicinityContainer(this, false);
        m_Body.Insert(m_VicinityContainer);
        m_ActiveIndex = 0;
        
        WidgetEventHandler.GetInstance().RegisterOnChildAdd(m_MainWidget, this, &quot;OnChildAdd&quot;);
        WidgetEventHandler.GetInstance().RegisterOnChildRemove(m_MainWidget, this, &quot;OnChildRemove&quot;);
        RecomputeOpenedContainers();
