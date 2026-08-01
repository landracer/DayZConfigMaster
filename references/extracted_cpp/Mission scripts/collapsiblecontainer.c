// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containers/collapsiblecontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CollapsibleContainer: Container
{
    protected ref CollapsibleHeader     m_CollapsibleHeader;
    protected bool                      m_Hidden;
    
    void CollapsibleContainer( LayoutHolder parent, int sort = -1 )
    {
        m_MainWidget.Show( true );
 
        if ( sort > -1 )
            m_MainWidget.SetSort( sort + 2 );
        
        m_MainWidget = m_MainWidget.FindAnyWidget( &quot;header&quot; );
        
        m_Body = new array<ref LayoutHolder>;
        m_CollapsibleHeader = new CollapsibleHeader( this, &quot;CollapseButtonOnMouseButtonDown&quot; );
