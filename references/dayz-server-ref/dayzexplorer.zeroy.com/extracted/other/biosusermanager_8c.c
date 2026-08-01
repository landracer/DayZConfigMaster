// ======================================================================
// File: biosusermanager_8c_source.html
// Category: other
// ======================================================================

enum EBiosJoinType
{
INVITE,
ACTIVITY,
};
class BiosUser
{
proto native owned string GetName();
proto native owned string GetUid();
proto native BiosClientServices GetClientServices();
proto native bool IsOnline();
};
class BiosUserManager
{
proto native BiosUser GetTitleInitiator();
proto native EBiosError GetUserList(array<ref BiosUser> user_list);
proto native EBiosError LogOnUserAsync(BiosUser user);
proto native EBiosError PickUserAsync();
proto native EBiosError ParseJoinAsync(string join_data);
proto native EBiosError ParsePartyAsync(string party_data);
proto native bool SelectUser(BiosUser user);
proto native BiosUser GetSelectedUser();
proto native EBiosError GetUserDatabaseIdAsync();
bool SelectUserEx(BiosUser user)
{
bool success = false;
BiosUser selectedUser = GetSelectedUser();
if (selectedUser && selectedUser != user && g_Game.GetGameState() != DayZGameState.MAIN_MENU)
{
success = SelectUser(user);
g_Game.DisconnectSessionEx(DISCONNECT_SESSION_FLAGS_FORCE & ~DisconnectSessionFlags.SELECT_USER);
}
else
success = SelectUser(user);
if (!success)
{
// Give it a moment and check again (there&#39;s a brief moment where the services become unavailable during the user transition)
GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckUserSwitchResult, 100, false, user);
}
return success;
}
void CheckUserSwitchResult(BiosUser expectedUser)
{
if (GetSelectedUser() != expectedUser)
{
NotificationSystem.AddNotification(NotificationType.GENERIC_ERROR, NotificationSystem.DEFAULT_TIME_DISPLAYED);
}
}
void OnUserDatabaseId(string dbID, EBiosError error)
{
if ( !OnlineServices.ErrorCaught( error ) )
{
g_Game.SetDatabaseID( dbID );
}
}
void OnUserLoggedOn(EBiosError error)
{
if ( OnlineServices.ErrorCaught( error ) )
{
g_Game.GetInput().ResetActiveGamepad();
g_Game.DisconnectSessionEx(DISCONNECT_SESSION_FLAGS_FORCE);
}
}
void OnUserPicked(BiosUser user, EBiosError error)
{
if ( !user )
{
g_Game.GetInput().ResetActiveGamepad();
g_Game.GamepadCheck();
}
else if ( !OnlineServices.ErrorCaught( error ) )
{
if (SelectUserEx( user ))
{
Mission mission = g_Game.GetMission();
if ( mission )
mission.Reset();
OnGameNameChanged( user );
g_Game.SelectUser(g_Game.GetInput().GetUserGamepad(user));
}
}
}
void OnLoggedOn(BiosUser user)
{
if ( user && GetSelectedUser() == user )
g_Game.SelectUser(g_Game.GetInput().GetUserGamepad(user));
}
void OnLoggedOff(BiosUser user)
{
OnSignedOut( user );
}
void OnSignedIn(BiosUser user)
{
}
void OnSignedOut(BiosUser user)
{
if ( user == GetSelectedUser() )
{
SelectUserEx( null );
g_Game.GetInput().ResetActiveGamepad();
g_Game.DisconnectSessionEx(DISCONNECT_SESSION_FLAGS_FORCE & ~DisconnectSessionFlags.SELECT_USER);
}
}
void OnJoin(EBiosJoinType type, BiosUser joiner, string handle, string joinee, EBiosError error)
{
if ( !OnlineServices.ErrorCaught( error ) )
{
OnlineServices.SetBiosUser( joiner );
SelectUserEx( joiner );
OnlineServices.SetSessionHandle( handle );
if ( g_Game.GetGameState() == DayZGameState.IN_GAME )
{
g_Game.SetLoadState( DayZLoadState.JOIN_START );
OnlineServices.GetSession();
}
else
{
if ( g_Game.GetUIManager() && g_Game.GetInput().IsActiveGamepadSelected() )
{
g_Game.GetUIManager().CloseMenu( MENU_TITLE_SCREEN );
g_Game.GetInput().IdentifyGamepad( GamepadButton.BUTTON_NONE );
}
g_Game.SetLoadState( DayZLoadState.JOIN_START );
g_Game.GamepadCheck();
}
}
else
{
g_Game.DisconnectSessionEx(DISCONNECT_SESSION_FLAGS_JOIN);
}
}
void OnPartyHost(BiosUser host, array<string> invitee_list, EBiosError error)
{
#ifdef PLATFORM_PS4
if (host)
#endif
{
SelectUserEx(host);
#ifdef PLATFORM_PS4
if (!host.IsOnline())
{
LogOnUserAsync( host );
}
#endif
}
if (g_Game.GetUIManager())
{
g_Game.GetUIManager().CloseMenu(MENU_TITLE_SCREEN);
}
OnlineServices.SetPendingInviteList( invitee_list );
if (g_Game.GetGameState() != DayZGameState.IN_GAME && g_Game.GetGameState() != DayZGameState.CONNECTING)
{
if (!g_Game.GetUIManager().GetMenu() || g_Game.GetUIManager().GetMenu().GetID() != MENU_MAIN)
{
g_Game.GetUIManager().EnterScriptedMenu(MENU_MAIN, g_Game.GetUIManager().GetMenu());
}
g_Game.SetGameState( DayZGameState.PARTY );
g_Game.SetLoadState( DayZLoadState.PARTY_START );
g_Game.GamepadCheck();
}
}
void OnGameNameChanged(BiosUser user)
{
if ( user == GetSelectedUser() )
{
g_Game.SetPlayerName( user.GetName() );
#ifdef PLATFORM_CONSOLE
g_Game.SetPlayerGameName( user.GetName() );
#endif
if ( g_Game.GetUIManager().GetMenu() )
{
g_Game.GetUIManager().GetMenu().Refresh();
}
}
}
BiosUser GetUser( string user_id )
{
array<ref BiosUser> user_list = new array<ref BiosUser>;
GetUserList( user_list );
foreach ( BiosUser user : user_list )
{
if ( user.GetUid() == user_id )
{
return user;
}
}
return null;
}
};