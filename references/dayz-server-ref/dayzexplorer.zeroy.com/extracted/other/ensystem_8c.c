// ======================================================================
// File: ensystem_8c_source.html
// Category: other
// ======================================================================

proto void GetHourMinuteSecond(out int hour, out int minute, out int second);
proto void GetYearMonthDay(out int year, out int month, out int day);
proto void GetHourMinuteSecondUTC(out int hour, out int minute, out int second);
proto void GetYearMonthDayUTC(out int year, out int month, out int day);
proto string GetProfileName();
proto string GetMachineName();
proto native int TickCount(int prev);
proto native void MemoryValidation(bool enable);
proto bool GetCLIParam(string param, out string val);
proto native bool IsCLIParam(string param);
#ifdef ENF_DONE
proto native void StartVideo(string name);
proto native void StopVideo();
#endif
enum KeyCode
{
KC_ESCAPE,
KC_1,
KC_2,
KC_3,
KC_4,
KC_5,
KC_6,
KC_7,
KC_8,
KC_9,
KC_0,
KC_MINUS,
KC_EQUALS,
KC_BACK,
KC_TAB,
KC_Q,
KC_W,
KC_E,
KC_R,
KC_T,
KC_Y,
KC_U,
KC_I,
KC_O,
KC_P,
KC_LBRACKET,
KC_RBRACKET,
KC_RETURN,
KC_LCONTROL,
KC_A,
KC_S,
KC_D,
KC_F,
KC_G,
KC_H,
KC_J,
KC_K,
KC_L,
KC_SEMICOLON,
KC_APOSTROPHE,
KC_GRAVE,
KC_LSHIFT,
KC_BACKSLASH,
KC_Z,
KC_X,
KC_C,
KC_V,
KC_B,
KC_N,
KC_M,
KC_COMMA,
KC_PERIOD,
KC_SLASH,
KC_RSHIFT,
KC_MULTIPLY,
KC_LMENU,
KC_SPACE,
KC_CAPITAL,
KC_F1,
KC_F2,
KC_F3,
KC_F4,
KC_F5,
KC_F6,
KC_F7,
KC_F8,
KC_F9,
KC_F10,
KC_NUMLOCK,
KC_SCROLL,
KC_NUMPAD7,
KC_NUMPAD8,
KC_NUMPAD9,
KC_SUBTRACT,
KC_NUMPAD4,
KC_NUMPAD5,
KC_NUMPAD6,
KC_ADD,
KC_NUMPAD1,
KC_NUMPAD2,
KC_NUMPAD3,
KC_NUMPAD0,
KC_DECIMAL,
KC_OEM_102,
KC_F11,
KC_F12,
KC_NUMPADEQUALS,
KC_PREVTRACK,
KC_AT,
KC_COLON,
KC_UNDERLINE,
KC_STOP,
KC_AX,
KC_UNLABELED,
KC_NEXTTRACK,
KC_NUMPADENTER,
KC_RCONTROL,
KC_MUTE,
KC_CALCULATOR,
KC_PLAYPAUSE,
KC_MEDIASTOP,
KC_VOLUMEDOWN,
KC_VOLUMEUP,
KC_WEBHOME,
KC_NUMPADCOMMA,
KC_DIVIDE,
KC_SYSRQ,
KC_RMENU,
KC_PAUSE,
KC_HOME,
KC_UP,
KC_PRIOR,
KC_LEFT,
KC_RIGHT,
KC_END,
KC_DOWN,
KC_NEXT,
KC_INSERT,
KC_DELETE,
KC_LWIN,
KC_RWIN,
KC_APPS,
KC_POWER,
KC_SLEEP,
KC_WAKE,
KC_MEDIASELECT
};
proto native int KeyState(KeyCode key);
proto native void ClearKey(KeyCode key);
//proto native int GetDefKey(string name);
//proto native int DefKeyState(int defkey, bool clear);
enum MouseState
{
LEFT,
RIGHT,
MIDDLE,
X,
Y,
WHEEL
};
//const int MB_PRESSED_MASK
proto native int GetMouseState(MouseState index);
// Gets current mouse position
proto void GetMousePos(out int x, out int y);
// Gets current screen size (resolution)
proto void GetScreenSize(out int x, out int y);
enum GamepadButton
{
BUTTON_NONE,
MENU,
VIEW,
A,
B,
X,
Y,
PAD_UP,
PAD_DOWN,
PAD_LEFT,
PAD_RIGHT,
SHOULDER_LEFT,
SHOULDER_RIGHT,
THUMB_LEFT,
THUMB_RIGHT
};
enum GamepadAxis
{
LEFT_THUMB_HORIZONTAL,
LEFT_THUMB_VERTICAL,
RIGHT_THUMB_HORIZONTAL,
RIGHT_THUMB_VERTICAL,
LEFT_TRIGGER,
RIGHT_TRIGGER,
};
proto native int GetGamepadButton(GamepadButton button);
proto native float GetGamepadAxis(GamepadAxis axis);
//----------------------------------------------
enum FileMode
{
READ,
WRITE,
APPEND
};
typedef int[] ParseHandle;
typedef int[] FileHandle;
proto native ParseHandle BeginParse(string filename);
proto int ParseLine(ParseHandle tp, int num, string tokens[]);
proto native void EndParse(ParseHandle file);
proto bool FileExist(string name);
proto FileHandle OpenFile(string name, FileMode mode);
proto int ReadFile(FileHandle file, void param_array, int length);
proto void CloseFile(FileHandle file);
proto void FPrint(FileHandle file, void var);
proto void FPrintln(FileHandle file, void var);
proto int FGets(FileHandle file, string var);
typedef int[] FindFileHandle;
enum FileAttr
{
DIRECTORY,
HIDDEN,
READONLY,
INVALID
};
enum FindFileFlags
{
DIRECTORIES,
ARCHIVES,
ALL
}
proto FindFileHandle FindFile(string pattern, out string fileName, out FileAttr fileAttributes, FindFileFlags flags);
proto bool FindNextFile(FindFileHandle handle, out string fileName, out FileAttr fileAttributes);
proto native void CloseFindFile(FindFileHandle handle);
proto native bool MakeDirectory(string name);
proto native bool DeleteFile(string name);
proto native bool CopyFile(string sourceName, string destName);