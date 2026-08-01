// Source: F:/Games/Dayz/scripts/3_game/gui/spacers/spacerbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class SpacerBase : ScriptedWidgetEventHandler
{
    protected Widget m_root;
    protected int m_count;
 
    // -----------------------------------------------------------
    void OnWidgetScriptInit(Widget w)
    {
        m_root = w;
        m_count = 0;
    
        Widget child = m_root.GetChildren();
        while (child)
        {
            m_count++;
            child.SetFlags(WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE, false);
            child = child.GetSibling();
        }
    
        m_root.SetHandler(this);
