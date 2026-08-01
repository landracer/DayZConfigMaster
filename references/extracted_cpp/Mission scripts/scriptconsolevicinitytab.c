// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptconsolevicinitytab.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ScriptConsoleVicinityTab : ScriptConsoleTabBase
{
    protected static float m_Distance = 30;
    protected static Object m_Selected = null;
    
    protected TextListboxWidget     m_VicinityListbox;
    protected ButtonWidget          m_WatchItem;    
    protected EditBoxWidget         m_DistanceWidget;
    protected ref array<Object>     m_VicinityItems = new array<Object>;
 
    protected ref ContextMenu       m_ContextMenu;
    
    void ScriptConsoleVicinityTab(Widget root, ScriptConsole console, Widget button, ScriptConsoleTabBase parent = null)
    {
        m_VicinityListbox = TextListboxWidget.Cast(root.FindAnyWidget(&quot;VicinityListbox&quot;));
        m_WatchItem = ButtonWidget.Cast(root.FindAnyWidget(&quot;WatchItemButton&quot;));
        m_DistanceWidget = EditBoxWidget.Cast(root.FindAnyWidget(&quot;DistanceInput&quot;));
        
        m_DistanceWidget.SetText(m_Distance.ToString());
 
        m_ContextMenu = new ContextMenu();
        m_ContextMenu.Init(root.FindAnyWidget(&quot;VicinityContextMenu&quot;), true);
 
        m_ContextMenu.SetSize(1, 1);
        m_ContextMenu.Show(0, 0);
        
        RenderList();
