// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/controlmapping/controlmappingkeybinds.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TutorialKeybinds extends ScriptedWidgetEventHandler
{
    protected Widget                        m_Root;
    protected Widget                        m_DetailsRoot;
    
    protected TutorialsMenu                 m_Menu;
    
    void TutorialKeybinds(Widget parent, TutorialsMenu menu)
    {
        m_Root                          = g_Game.GetWorkspace().CreateWidgets(GetLayoutName(), parent);
        m_Menu                          = menu;
        
        Rebuild();
        m_Root.SetHandler(this);
