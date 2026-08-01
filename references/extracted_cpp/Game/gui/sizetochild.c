// Source: F:/Games/Dayz/scripts/3_game/gui/containers/sizetochild.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SizeToChild extends ScriptedWidgetEventHandler
{
    reference string        m_ChildName;
    reference float         m_HorizontalOffset;
    reference float         m_VerticalOffset;
    reference bool          m_ResizeHorizontal;
    reference bool          m_ResizeVertical;
    
    protected Widget        m_Root;
    protected Widget        m_Child;
    
    protected static bool   m_IgnoredBool;
    
    void OnWidgetScriptInit(Widget w)
    {
        m_Root  = w;
        
        m_Child = m_Root.FindAnyWidget( m_ChildName );
        if ( m_Child )
        {
            ResizeParentToChild();
        }
