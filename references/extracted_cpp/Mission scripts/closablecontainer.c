// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containers/closablecontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ClosableContainer extends Container
{
    protected ref ClosableHeader    m_ClosableHeader;
    protected bool                  m_LockCargo;
 
    void ClosableContainer( LayoutHolder parent, int sort = -1 )
    {
        m_Body              = new array<ref LayoutHolder>;
        m_ClosableHeader    = new ClosableHeader( this, &quot;CloseButtonOnMouseButtonDown&quot; );
        
        if( sort > -1 )
            m_RootWidget.SetSort( sort + 2 );
        
        m_MainWidget = m_MainWidget.FindWidget( &quot;body&quot; );
