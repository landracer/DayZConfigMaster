// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper/plugindrawcheckerboard.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// quick and dirty way for displaying of checker overlay on screen
// - userd for camera settings primarily
class PluginDrawCheckerboard extends PluginBase
{
    private ref Widget  m_MainWindow;
    private bool        m_IsActive;
    
    
    void PluginDrawCheckerboard()
    {
        CreateWidgetOverlay();
