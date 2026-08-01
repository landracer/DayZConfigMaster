// Source: F:/Games/Dayz/scripts/5_mission/gui/chat/chat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// #include &quot;Scripts\Classes\Gui\ChatLine.c&quot;

 
class Chat
{
    const int LINE_COUNT = 12;
 
    protected Widget                    m_RootWidget;
    protected int                       m_LineHeight;
    protected int                       m_LastLine;
    protected ref array<ref ChatLine>   m_Lines;
 
    void Chat()
    {
        m_Lines = new array<ref ChatLine>;
