// Source: F:/Games/Dayz/scripts/5_mission/gui/titlescreenmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class TitleScreenMenu extends UIScriptedMenu
{
    RichTextWidget m_TextPress;
    
    void TitleScreenMenu()
    {
        g_Game.SetGameState(DayZGameState.MAIN_MENU);
        g_Game.SetLoadState(DayZLoadState.MAIN_MENU_START);
