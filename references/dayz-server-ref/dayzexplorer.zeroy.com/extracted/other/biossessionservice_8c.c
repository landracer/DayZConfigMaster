// ======================================================================
// File: biossessionservice_8c_source.html
// Category: other
// ======================================================================

class BiosSessionService
{
protected int m_GetSessionAttempts;
string m_CurrentHandle;
proto native EBiosError EnterGameplaySessionAsync(string session_address, int session_port);
proto native EBiosError LeaveGameplaySessionAsync(string session_address, int session_port);
proto native void OnSessionPlayerListUpdate(array<string> newPlayers);
void TryGetSession( string join_handle = "" )
{
if ( join_handle != "" )
{
m_GetSessionAttempts = 0;
m_CurrentHandle = join_handle;
}
if ( m_GetSessionAttempts < 10 )
GetSessionAsync( m_CurrentHandle );
else
g_Game.DisconnectSessionEx(DISCONNECT_SESSION_FLAGS_JOIN);
}
proto native EBiosError GetSessionAsync(string join_handle);
proto native EBiosError SetGameplayActivityAsync(string session_address, int session_port);
//proto native EBiosError SetLobbyActivityAsync(...);
proto native EBiosError ClearActivityAsync();
proto native EBiosError ShowInviteToGameplaySessionAsync(string session_address, int session_port);
proto native EBiosError InviteToGameplaySessionAsync(string session_address, int session_port, array<string> invitee_list);
proto native void SetMultiplayState(bool is_active);
void OnSetActivity(EBiosError error)
{
OnlineServices.ErrorCaught( error );
}
void OnClearActivity(EBiosError error)
{
}
void OnGetGameplaySession(string session_address, int session_port)
{
m_GetSessionAttempts = 0;
switch (g_Game.GetGameState())
{
case DayZGameState.IN_GAME:
{
string addr;
int port;
bool found = g_Game.GetHostAddress( addr, port );
if (addr != session_address || port != session_port )
{
if (found)
{
OnlineServices.SetInviteServerInfo( session_address, session_port );
g_Game.GetUIManager().CloseAll();
if (!g_Game.GetUIManager().EnterScriptedMenu( MENU_INVITE_TIMER, null ))
{
NotificationSystem.AddNotification( NotificationType.CONNECT_FAIL_GENERIC, NotificationSystem.DEFAULT_TIME_DISPLAYED );
}
}
else
{
NotificationSystem.AddNotification( NotificationType.JOIN_FAIL_GET_SESSION, NotificationSystem.DEFAULT_TIME_DISPLAYED );
}
}
else
{
NotificationSystem.AddNotification( NotificationType.INVITE_FAIL_SAME_SERVER, NotificationSystem.DEFAULT_TIME_DISPLAYED, "#ps4_already_in_session" );
}
break;
}
case DayZGameState.CONNECTING:
{
g_Game.DisconnectSessionEx(DISCONNECT_SESSION_FLAGS_FORCE);
// Intentionally no break, fall through to connecting
}
default:
{
g_Game.ConnectFromJoin( session_address, session_port );
break;
}
}
}
/*void OnGetLobbySession(...)
{
}*/
void OnGetSessionError(EBiosError error)
{
OnlineServices.ErrorCaught( error );
m_GetSessionAttempts++;
#ifdef PLATFORM_MSSTORE
GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( TryGetSession, 100, false, "" );
#endif
#ifdef PLATFORM_XBOX
g_Game.GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( TryGetSession, 100, false, "" );
#endif
#ifdef PLATFORM_PS4
g_Game.DisconnectSessionEx(DISCONNECT_SESSION_FLAGS_JOIN);
#endif
}
void OnEnterGameplaySession(string session_address, int session_port, EBiosError error)
{
if ( !OnlineServices.ErrorCaught( error ) )
{
SetGameplayActivityAsync( session_address, session_port );
if ( OnlineServices.GetPendingInviteList() )
InviteToGameplaySessionAsync( session_address, session_port, OnlineServices.GetPendingInviteList() );
//OnlineServices.GetCurrentServerInfo(session_address, session_port);
}
}
void OnLeaveGameplaySession(EBiosError error)
{
}
void OnShowInviteToGameplaySession(EBiosError error)
{
OnlineServices.ErrorCaught( error );
}
void OnInviteToGameplaySession(EBiosError error)
{
}
array<string> GetSessionPlayerList()
{
return ClientData.GetSimplePlayerList();
}
void GetSessionPlayerListEx(TStringArray outPlayerList)
{
TStringArray playerList = GetSessionPlayerList();
outPlayerList.Copy(playerList);
}
};