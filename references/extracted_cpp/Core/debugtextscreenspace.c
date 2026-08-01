// Source: F:/Games/Dayz/scripts/1_core/debug/debugtextscreenspace.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
class DebugTextScreenSpace: DebugText
{
    // Instantiate using the Create method
    private void DebugTextScreenSpace();
    void ~DebugTextScreenSpace();
 
    proto external void SetPosition(float x, float y);
    proto external vector GetPosition();
    static proto DebugTextScreenSpace Create(string text, DebugTextFlags flags, float x, float y, float size = 20.0, int color = 0xFFFFFFFF, int bgColor = 0x00000000, int priority = 1000);
