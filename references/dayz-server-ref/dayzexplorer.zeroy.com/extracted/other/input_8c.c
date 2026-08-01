// ======================================================================
// File: input_8c_source.html
// Category: other
// ======================================================================

enum EInputDeviceType
{
UNKNOWN,
MOUSE_AND_KEYBOARD,
CONTROLLER
};
//-----------------------------------------------------------------------------
class Input
{
// input locking
proto native void ChangeGameFocus(int add, int input_device = -1);
proto native void ResetGameFocus(int input_device = -1);
proto native bool HasGameFocus(int input_device = -1);
// actions
proto native int GetActionGroupsCount();
proto native int GetActionGroupSize(int group_index);
proto int GetActionGroupName(int group_index, out string name);
proto int GetActionDesc(int action_index, out string desc);
// getting action state
proto native float LocalValue_ID(int action, bool check_focus = true);
proto native float LocalValue(string action, bool check_focus = true);
proto native bool LocalPress_ID(int action, bool check_focus = true);
proto native bool LocalPress(string action, bool check_focus = true);
proto native bool LocalRelease_ID(int action, bool check_focus = true);
proto native bool LocalRelease(string action, bool check_focus = true);
proto native bool LocalHold_ID(int action, bool check_focus = true);
proto native bool LocalHold(string action, bool check_focus = true);
proto native bool LocalDbl_ID(int action, bool check_focus = true);
proto native bool LocalDbl(string action, bool check_focus = true);
proto native void DisableKey(int key);
proto native void EnableMouseAndKeyboard(bool enable);
proto native bool IsEnabledMouseAndKeyboard();
proto native void EnableGamepad(bool enable);
// NOTE: not actually supported, just keeping naming consistent.
// Required as we need to disable gamepad on windows when in the server browser to prevent
// the client from freezing on gamepad queries while refreshing the server list .
proto native bool IsEnabledGamepad();
proto native bool IsEnabledMouseAndKeyboardEvenOnServer();
proto native bool IsMouseConnected();
proto native bool IsKeyboardConnected();
proto native int GetCurrentProfile();
// gets currently selected profile keys for action
proto native void GetCurrentProfileActionKeys(int action_index, out TIntArray keys);
proto int GetProfileName(int profile_index, out string name);
proto native int GetProfilesCount();
proto native int SetProfile(int index);
// devices - joystick only!
proto native int GetDevicesCount();
proto int GetDeviceName(int device_index, out string name);
proto native int IsDeviceXInput(int device_index);
proto native int IsDeviceEnabled(int device_index);
proto native void SetDeviceEnabled(int device_index, bool enabled);
proto bool GetGamepadThumbDirection(GamepadButton thumbButton, out float angle, out float value);
proto native void ResetActiveGamepad();
proto native void SelectActiveGamepad(int gamepad);
proto native void GetGamepadList(out array<int> gamepads);
proto void GetGamepadUser(int gamepad, out BiosUser user);
proto native void IdentifyGamepad(GamepadButton button);
proto native bool IsActiveGamepadSelected();
bool IsAnyInputDeviceActive()
{
return IsActiveGamepadSelected() || IsMouseConnected() || IsKeyboardConnected();
}
bool AreAllAllowedInputDevicesActive(out array<int> unavailableDeviceList = null)
{
bool passed = true;
bool gamepad = IsActiveGamepadSelected();
bool mouse = IsMouseConnected();
bool keyboard = IsKeyboardConnected();
bool MnKEnabled;
if (g_Game.GetGameState() != DayZGameState.IN_GAME)
{
MnKEnabled = IsEnabledMouseAndKeyboard();
}
else if (g_Game.GetGameState() != DayZGameState.MAIN_MENU)
{
MnKEnabled = IsEnabledMouseAndKeyboardEvenOnServer();
}
else
{
return true;
}
if (!MnKEnabled)
{
if (!gamepad)
{
passed = false;
FillUnavailableDeviceArray(EUAINPUT_DEVICE_CONTROLLER,unavailableDeviceList);
}
}
else
{
if (!gamepad)
{
if (!mouse)
{
passed = false;
FillUnavailableDeviceArray(EUAINPUT_DEVICE_MOUSE,unavailableDeviceList);
}
if (!keyboard)
{
passed = false;
FillUnavailableDeviceArray(EUAINPUT_DEVICE_KEYBOARD,unavailableDeviceList);
}
if (!passed)
{
FillUnavailableDeviceArray(EUAINPUT_DEVICE_CONTROLLER,unavailableDeviceList);
}
}
}
return passed;
}
void FillUnavailableDeviceArray(int device, inout array<int> filler)
{
if (filler)
{
filler.Insert(device);
}
}
void UpdateConnectedInputDeviceList()
{
g_Game.GetConnectedInputDeviceList().Clear();
if (IsActiveGamepadSelected())
g_Game.GetConnectedInputDeviceList().Insert(EUAINPUT_DEVICE_CONTROLLER);
if (IsMouseConnected())
g_Game.GetConnectedInputDeviceList().Insert(EUAINPUT_DEVICE_MOUSE);
if (IsKeyboardConnected())
g_Game.GetConnectedInputDeviceList().Insert(EUAINPUT_DEVICE_KEYBOARD);
}
proto native EInputDeviceType GetCurrentInputDevice();
proto native GamepadButton GetEnterButton();
void OnGamepadConnected(int gamepad)
{
if (!g_Game)
return;
#ifdef PLATFORM_PS4
BiosUser user;
GetGamepadUser( gamepad, user );
if (user && user == g_Game.GetUserManager().GetSelectedUser())
{
SelectActiveGamepad(gamepad);
Mission mission = g_Game.GetMission();
if (mission)
mission.GetOnInputDeviceConnected().Invoke(EUAINPUT_DEVICE_CONTROLLER); //only for PS, xbox handles it on identification
}
#endif
#ifdef PLATFORM_XBOX
if (gamepad == g_Game.GetPreviousGamepad())
{
SelectActiveGamepad(g_Game.GetPreviousGamepad());
Mission mission = g_Game.GetMission();
if (mission)
mission.GetOnInputDeviceConnected().Invoke(EUAINPUT_DEVICE_CONTROLLER); //only for PS, xbox handles it on identification
}
#endif
}
void OnGamepadDisconnected(int gamepad)
{
if (!g_Game)
return;
if (IsInactiveGamepadOrUserSelected(gamepad))
{
UpdateConnectedInputDeviceList();
if (!g_Game.IsLoading())
{
DayZLoadState state = g_Game.GetLoadState();
if (state != DayZLoadState.MAIN_MENU_START && state != DayZLoadState.MAIN_MENU_USER_SELECT)
{
Mission mission = g_Game.GetMission();
if (mission)
mission.GetOnInputDeviceDisconnected().Invoke(EUAINPUT_DEVICE_CONTROLLER);
}
}
}
}
void OnGamepadIdentification(int gamepad)
{
if (!g_Game)
return;
if (gamepad > -1)
{
DayZLoadState state = g_Game.GetLoadState();
UpdateConnectedInputDeviceList();
SelectActiveGamepad(gamepad);
g_Game.SelectUser(gamepad);
g_Game.SetPreviousGamepad(gamepad);
Mission mission = g_Game.GetMission();
if (state == DayZLoadState.MAIN_MENU_START || state == DayZLoadState.MAIN_MENU_USER_SELECT)
{
if (mission)
mission.Reset();
}
if (mission && mission.GetOnInputDeviceConnected())
mission.GetOnInputDeviceConnected().Invoke(EUAINPUT_DEVICE_CONTROLLER);
}
}
int GetUserGamepad( BiosUser user )
{
array<int> gamepads = new array<int>;
GetGamepadList( gamepads );
for( int i = 0; i < gamepads.Count(); i++ )
{
BiosUser user2;
GetGamepadUser( gamepads[i], user2 );
if( user == user2 )
return gamepads[i];
}
return -1;
}
bool IsInactiveGamepadOrUserSelected( int gamepad = -1 )
{
if (!g_Game)
return false;
#ifdef PLATFORM_CONSOLE
if (!g_Game.GetUserManager())
return false;
#ifdef PLATFORM_XBOX
return !IsActiveGamepadSelected();
#endif
#ifdef PLATFORM_PS4
BiosUser user;
GetGamepadUser( gamepad, user );
return (user == g_Game.GetUserManager().GetSelectedUser());
#endif
#endif
return false;
}
void OnMouseConnected()
{
if (!g_Game)
return;
UpdateConnectedInputDeviceList();
if (!g_Game.IsLoading() && g_Game.GetMission())
{
DayZLoadState state = g_Game.GetLoadState();
if (state != DayZLoadState.MAIN_MENU_START && state != DayZLoadState.MAIN_MENU_USER_SELECT)
{
g_Game.GetMission().GetOnInputDeviceConnected().Invoke(EUAINPUT_DEVICE_MOUSE);
}
}
}
void OnMouseDisconnected()
{
if (!g_Game)
return;
UpdateConnectedInputDeviceList();
if (!g_Game.IsLoading() && g_Game.GetMission())
{
DayZLoadState state = g_Game.GetLoadState();
if (state != DayZLoadState.MAIN_MENU_START && state != DayZLoadState.MAIN_MENU_USER_SELECT)
{
g_Game.GetMission().GetOnInputDeviceDisconnected().Invoke(EUAINPUT_DEVICE_MOUSE);
}
}
}
void OnKeyboardConnected()
{
if (!g_Game)
return;
UpdateConnectedInputDeviceList();
if (!g_Game.IsLoading() && g_Game.GetMission())
{
DayZLoadState state = g_Game.GetLoadState();
if (state != DayZLoadState.MAIN_MENU_START && state != DayZLoadState.MAIN_MENU_USER_SELECT)
{
g_Game.GetMission().GetOnInputDeviceConnected().Invoke(EUAINPUT_DEVICE_KEYBOARD);
}
}
}
void OnKeyboardDisconnected()
{
if (!g_Game)
return;
UpdateConnectedInputDeviceList();
if (!g_Game.IsLoading() && g_Game.GetMission())
{
DayZLoadState state = g_Game.GetLoadState();
if (state != DayZLoadState.MAIN_MENU_START && state != DayZLoadState.MAIN_MENU_USER_SELECT)
{
g_Game.GetMission().GetOnInputDeviceDisconnected().Invoke(EUAINPUT_DEVICE_KEYBOARD);
}
}
}
void OnLastInputDeviceChanged(EInputDeviceType inputDevice)
{
if (!g_Game)
return;
if (g_Game.GetMission())
{
g_Game.GetMission().GetOnInputDeviceChanged().Invoke(inputDevice);
}
}
};