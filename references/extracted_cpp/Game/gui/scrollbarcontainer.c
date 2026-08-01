// Source: F:/Games/Dayz/scripts/3_game/gui/containers/scrollbarcontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class ScrollBarContainer : ScriptedWidgetEventHandler
{
    reference bool Invert;
    protected Widget Content;
    protected Widget ScrollBar;
    protected Widget Scroller;
    protected Widget m_root;
    
    const int WHEEL_STEP = 20;
    protected float     m_root_height;
    protected float     m_content_height;
    protected float     m_position;
    protected bool      m_scrolling;
    protected float     m_scrolling_start_pos;
    protected int       m_scrolling_mouse_pos;
        
    void ~ScrollBarContainer()
    {
        //if(g_Game != NULL)
        //g_Game.GetDragQueue().RemoveCalls(this);
