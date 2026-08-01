// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/layoutholder.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LayoutHolder extends ScriptedWidgetEventHandler
{
    protected Widget                m_MainWidget;
    protected Widget                m_RootWidget;
    protected Widget                m_ParentWidget;
    protected LayoutHolder          m_Parent;
    protected string                m_LayoutName;
    
    protected bool                  m_IsActive;
    protected bool                  m_ImmedUpdate;
    protected bool                  m_TooltipOwner;
    
    protected EntityAI              m_am_entity1, m_am_entity2;
 
    void UpdateInterval();
    void SetLayoutName();
    
    // Override this and set m_ImmedUpdate to true if you need the widget to update on construction
    // Had to be done this way since adding it to the constructor parameters would break mods..
    void SetImmedUpdate()
    {
        m_ImmedUpdate = false;
