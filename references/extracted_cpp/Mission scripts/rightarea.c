// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/areas/rightarea.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RightArea: Container
{
    ref PlayerContainer m_PlayerContainer;
    protected Widget    m_ContentParent;
 
    protected Widget                            m_SlotsHeader;
    protected Widget                            m_SlotsContent;
    protected ScrollWidget                      m_ScrollWidget;
    protected Widget                            m_UpIcon;
    protected Widget                            m_DownIcon;
    
    protected ref SizeToChild                   m_ContentResize;
    protected bool                              m_ShouldChangeSize = true;
    protected bool                              m_ProcessGridMovement;
    
    void RightArea(LayoutHolder parent)
    {
        m_MainWidget.Show(true);
        
        m_SlotsHeader       = m_MainWidget.FindAnyWidget(&quot;SlotsHeader&quot;);
        m_SlotsContent      = m_MainWidget.FindAnyWidget(&quot;SlotsContent&quot;);
        m_ScrollWidget      = ScrollWidget.Cast(m_MainWidget.FindAnyWidget(&quot;Scroller&quot;));
        m_MainWidget        = m_MainWidget.FindAnyWidget(&quot;Content&quot;);
        //m_ContentParent   = m_RootWidget.FindAnyWidget(&quot;ContentParent&quot;);
        m_PlayerContainer   = new PlayerContainer(this, false);
        m_PlayerContainer.SetPlayer(PlayerBase.Cast(g_Game.GetPlayer()));
        m_Body.Insert(m_PlayerContainer);
        m_ActiveIndex = 0;
        m_ProcessGridMovement = false;
        
        m_UpIcon        = m_RootWidget.FindAnyWidget(&quot;Up&quot;);
        m_DownIcon      = m_RootWidget.FindAnyWidget(&quot;Down&quot;);
        
        //m_ContentParent.GetScript(m_ContentResize);
        
        RecomputeOpenedContainers();
