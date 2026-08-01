// Source: F:/Games/Dayz/scripts/3_game/global/errormodulehandler/errorproperties.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ErrorProperties
{
    const string EP_HEADER_FORMAT_STRING = &quot;%1 (%2)&quot;; 
    const string EP_MESSAGE_FORMAT_STRING = &quot;%1\n(%2)&quot;; 
    
    protected string m_Message; 
    protected string m_ServerMessage; 
    
    void ErrorProperties(string message, string serverMessage)
    {
        m_Message = message;
        m_ServerMessage = serverMessage;
