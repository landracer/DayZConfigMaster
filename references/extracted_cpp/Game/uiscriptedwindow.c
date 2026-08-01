// Source: F:/Games/Dayz/scripts/3_game/tools/uiscriptedwindow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIScriptedWindow
{
    Widget  m_WgtRoot;
    int m_Id;
 
    //---MOVE TO UIMANAGER WHEN FIXED
    static ref map<int, UIScriptedWindow> m_ActiveWindows;
    
    static void AddToActiveWindows( int id, UIScriptedWindow window )
    {
        if ( m_ActiveWindows == NULL )
        {
            m_ActiveWindows = new map<int, UIScriptedWindow>;
        }
            
        m_ActiveWindows.Insert( id, window );
