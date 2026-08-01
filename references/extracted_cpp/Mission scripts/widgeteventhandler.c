// Source: F:/Games/Dayz/scripts/5_mission/gui/widgeteventhandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WidgetEventHandler: ScriptedWidgetEventHandler
{
    private ref static WidgetEventHandler s_instance;
 
    protected ref map<Widget, ref Param> m_OnMouseButtonDownRegister;
    protected ref map<Widget, ref Param> m_OnMouseButtonUpRegister;
    protected ref map<Widget, ref Param> m_OnMouseWheel;
    protected ref map<Widget, ref Param> m_OnDropReceived;
    protected ref map<Widget, ref Param> m_OnDrag;
    protected ref map<Widget, ref Param> m_OnDrop;
    protected ref map<Widget, ref Param> m_OnDraggingOver;
    protected ref map<Widget, ref Param> m_OnMouseEnter;
    protected ref map<Widget, ref Param> m_OnMouseButtonLeave;
    protected ref map<Widget, ref Param> m_OnClick;
    protected ref map<Widget, ref Param> m_OnDoubleClick;
    protected ref map<Widget, ref Param> m_OnFocus;
    protected ref map<Widget, ref Param> m_OnFocusLost;
    protected ref map<Widget, ref Param> m_OnController;
    protected ref map<Widget, ref Param> m_OnChildAdd;
    protected ref map<Widget, ref Param> m_OnChildRemove;
 
    static WidgetEventHandler GetInstance()
    {
        return s_instance;
