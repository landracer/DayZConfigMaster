// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containeditems/slotscontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SlotsContainer: Container
{
    protected ref array<ref SlotsIcon>  m_Icons;
    int m_VisibleColumnCount;
    
    void SlotsContainer( LayoutHolder parent, EntityAI slot_parent ) 
    {
        m_Icons = new array<ref SlotsIcon>;
        for ( int i = 0; i < ITEMS_IN_ROW; i++ )
        {
            if ( GetRootWidget().FindAnyWidget( &quot;Icon&quot; + i ) )
                m_Icons.Insert( new SlotsIcon( this, GetRootWidget().FindAnyWidget( &quot;Icon&quot; + i ), i , slot_parent) );
            /*else
            {
                Widget child = GetRootWidget().GetChildren();
                while ( child )
                {
                    child = child.GetSibling();
                }
            }*/
        }
