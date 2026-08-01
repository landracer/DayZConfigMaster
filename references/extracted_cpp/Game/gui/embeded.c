// Source: F:/Games/Dayz/scripts/3_game/gui/embeded.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class EmbededButtonScript
{
    reference string Caption;
    void OnWidgetScriptInit(Widget w)
    {
        ButtonWidget button = ButtonWidget.Cast(w.GetChildren());
        button.SetText(Caption);
