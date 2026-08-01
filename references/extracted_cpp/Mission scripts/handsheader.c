// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containeditems/headers/handsheader.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HandsHeader: Header
{
    protected bool      m_ForceHideCollapseButtons;
    protected Widget    m_ItemHeader;
    
    void HandsHeader( LayoutHolder parent, string function_name )
    {
        m_DefaultFontSize   = 24;
        m_ItemHeader        = m_MainWidget.FindAnyWidget( &quot;hands_item_header&quot; );
        
        m_MainWidget        = GetMainWidget().FindAnyWidget( &quot;height_wrapper&quot; );
        m_DefaultColor      = GetMainWidget().GetColor();
        #ifdef PLATFORM_CONSOLE
        ShowCollapseButtons( false, true );
        #endif
