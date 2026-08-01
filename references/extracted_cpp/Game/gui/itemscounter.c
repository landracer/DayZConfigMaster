// Source: F:/Games/Dayz/scripts/3_game/gui/spacers/itemscounter.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// -----------------------------------------------------------
class ItemsCounter : ScriptedWidgetEventHandler
{
    bool NumberOfItems;
    
    void OnWidgetScriptInit(Widget w)
    {
    Widget child = w.GetChildren();
  
    while (child)
    {
        child = child.GetSibling();
        NumberOfItems++;
    }
