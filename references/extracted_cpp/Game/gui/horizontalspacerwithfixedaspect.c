// Source: F:/Games/Dayz/scripts/3_game/gui/spacers/horizontalspacerwithfixedaspect.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class HorizontalSpacerWithFixedAspect: ScriptedWidgetEventHandler
{
    protected Widget m_root;
    reference int border;
    reference int gap;
    reference float coef;
    float itemWidth;
    float itemHeight;
    
    // -----------------------------------------------------------
    void OnWidgetScriptInit(Widget w)
    {
        m_root = w;
        m_root.SetHandler(this);
