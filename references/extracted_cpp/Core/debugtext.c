// Source: F:/Games/Dayz/scripts/1_core/debug/debugtext.c Source File
// Extracted from DayZ Code Explorer by Zeroy



class DebugText: Managed
{
    private void DebugText();
    void ~DebugText();
 
    proto external string GetText();
    proto external float GetFontSize();
    proto external int GetTextColor();
    proto external int GetBackgroundColor();
    proto external int GetPriority();
    proto external void SetText(string text);
    proto external void SetTextColor(int color);
    proto external void SetFontSize(float size);
    proto external void SetBackgroundColor(int color);
    proto external void SetPriority(int priority);
