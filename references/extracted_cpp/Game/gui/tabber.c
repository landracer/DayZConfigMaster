// Source: F:/Games/Dayz/scripts/3_game/gui/tabber.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class Tabber : ScriptedWidgetEventHandler
{
    Widget ButtonsPanel;
    Widget ContentPanel;
    reference int SelectedTab;
    
    // -----------------------------------------------------------
    void OnWidgetScriptInit(Widget w)
    {
        w.SetHandler(this);
        SelectTab(SelectedTab);
