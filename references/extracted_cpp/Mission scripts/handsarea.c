// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/areas/handsarea.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HandsArea: Container
{
    protected ScrollWidget          m_Scroller;
    protected ref HandsContainer    m_HandsContainer;
    protected ref SizeToChild       m_HandsResizer;
    
    protected bool                  m_ShouldChangeSize = true;
 
    void HandsArea( LayoutHolder parent )
    {
        m_HandsContainer = new HandsContainer( this );
        m_Body.Insert(m_HandsContainer);
        
        if ( m_ImmedUpdate )
            m_MainWidget.Update();
        
        m_ParentWidget.GetScript( m_HandsResizer );
        m_Scroller = ScrollWidget.Cast( m_ParentWidget );
        RecomputeOpenedContainers();
