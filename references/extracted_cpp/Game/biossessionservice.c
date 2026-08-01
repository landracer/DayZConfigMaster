// Source: F:/Games/Dayz/scripts/3_game/services/biossessionservice.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BiosSessionService
{
    protected int               m_GetSessionAttempts;
    string                      m_CurrentHandle;
    

    proto native EBiosError EnterGameplaySessionAsync(string session_address, int session_port);
    

    proto native EBiosError LeaveGameplaySessionAsync(string session_address, int session_port);
    

    proto native void OnSessionPlayerListUpdate(array<string> newPlayers);
    

    void TryGetSession( string join_handle = &quot;&quot; )
    {
        if ( join_handle != &quot;&quot; )
        {
            m_GetSessionAttempts    = 0;
            m_CurrentHandle         = join_handle;
        }
        
        if ( m_GetSessionAttempts < 10 )
            GetSessionAsync( m_CurrentHandle );
        else
            g_Game.DisconnectSessionEx(DISCONNECT_SESSION_FLAGS_JOIN);
