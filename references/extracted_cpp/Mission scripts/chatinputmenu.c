// Source: F:/Games/Dayz/scripts/5_mission/gui/chat/chatinputmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ChatInputMenu extends UIScriptedMenu
{
    private EditBoxWidget m_edit_box;
    private TextWidget m_channel_text;
    private UAIDWrapper m_BackInputWrapper;
    private ref Timer m_close_timer;
 
    void ChatInputMenu()
    {
        m_close_timer = new Timer();
