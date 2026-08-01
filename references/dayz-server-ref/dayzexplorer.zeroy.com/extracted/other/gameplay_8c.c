// ======================================================================
// File: gameplay_8c_source.html
// Category: other
// ======================================================================

enum InventoryTraversalType
{
PREORDER,
INORDER,
POSTORDER,
LEVELORDER
};
const int INDEX_NOT_FOUND = -1;
//-----------------------------------------------------------------------------
typedef Serializer ParamsReadContext;
typedef Serializer ParamsWriteContext;
class JsonSerializer: Serializer
{
void JsonSerializer() {}
void ~JsonSerializer() {}
proto bool WriteToString(void variable_out, bool nice, out string result);
proto bool ReadFromString(void variable_in, string jsonString, out string error);
};
class ScriptRPC: ParamsWriteContext
{
void ScriptRPC();
void ~ScriptRPC();
proto native void Reset();
proto native void Send(Object target, int rpc_type, bool guaranteed,PlayerIdentity recipient = NULL);
};
class ScriptInputUserData : ParamsWriteContext
{
void ScriptInputUserData ();
void ~ScriptInputUserData ();
proto native void Reset ();
proto native void Send ();
proto native bool CopyFrom(ParamsReadContext other);
proto native static bool CanStoreInputUserData ();
};
class ScriptReadWriteContext
{
void ScriptReadWriteContext ();
void ~ScriptReadWriteContext ();
proto native ParamsWriteContext GetWriteContext ();
proto native ParamsReadContext GetReadContext ();
};
class ScriptRemoteInputUserData : ParamsWriteContext
{
void ScriptRemoteInputUserData ();
void ~ScriptRemoteInputUserData ();
proto native void Reset ();
};
class ScriptJunctureData : ParamsWriteContext
{
void ScriptJunctureData ();
void ~ScriptJunctureData ();
proto native void Reset ();
};
//-----------------------------------------------------------------------------
class MeleeCombatData
{
proto native int GetModesCount();
proto native owned string GetModeName(int index);
proto native owned string GetAmmoTypeName(int index);
proto native float GetModeRange(int index);
private void MeleeCombatData();
private void ~MeleeCombatData();
}
//-----------------------------------------------------------------------------
const string NullStringArray[1] = { "" };
//-----------------------------------------------------------------------------
class Selection
{
private void Selection() {}
private void ~Selection() {}
proto native owned string GetName();
proto native int GetVertexCount();
proto native int GetLODVertexIndex(int sel_vertex_index);
vector GetVertexPosition(LOD lod, int index)
{
int lodIndex = GetLODVertexIndex(index);
if (lodIndex == -1)
{
Error("Vertex doesn&#39;t exist");
return vector.Zero;
}
return lod.GetVertexPosition(lodIndex);
}
};
//-----------------------------------------------------------------------------
class LOD
{
// standard(BI) LOD names in p3d
static const string NAME_GEOMETRY = "geometry";
static const string NAME_VIEW = "view";
static const string NAME_FIRE = "fire";
static const string NAME_MEMORY = "memory";
static const string NAME_ROADWAY = "roadway";
private void LOD() {}
private void ~LOD() {}
proto native int GetSelectionCount();
proto native bool GetSelections(notnull out array<Selection> selections);
proto native vector GetVertexPosition(int vertex_index);
proto native owned string GetName(Object myObject);
Selection GetSelectionByName(string name)
{
array<Selection> selections = new array<Selection>;
GetSelections(selections);
for (int i = 0; i < selections.Count(); ++i)
{
string selection_name = selections.Get(i).GetName();
selection_name.ToLower();
name.ToLower();
if (selection_name == name)
{
return selections.Get(i);
}
}
return null;
}
proto native int GetPropertyCount();
proto native owned string GetPropertyName(int index);
proto native owned string GetPropertyValue(int index);
}
class Plant extends Object
{
};
/*
class ParamEntry
{
proto string GetString(string entryName);
proto int GetInt(string entryName);
proto float GetFloat(string entryName);
proto ref ParamEntry GetEntry(string entryName);
proto int GetNumChildren();
proto ref ParamEntry GetNumChildren(int n);
};
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class ProxyInventory extends ObjectTyped
{
};
//-----------------------------------------------------------------------------
class ProxySubpart extends Entity
{
};
//-----------------------------------------------------------------------------
// custom widgets
//-----------------------------------------------------------------------------
class ItemPreviewWidget: Widget
{
proto native void SetItem(EntityAI object);
proto native EntityAI GetItem();
proto native int GetView();
proto native void SetView(int viewIndex);
proto native void SetModelOrientation(vector vOrientation);
proto native vector GetModelOrientation();
proto native void SetModelPosition(vector vPos);
proto native vector GetModelPosition();
proto native void SetForceFlipEnable(bool enable);
proto native void SetForceFlip(bool value);
};
//-----------------------------------------------------------------------------
class PlayerPreviewWidget: Widget
{
proto native void UpdateItemInHands(EntityAI object);
proto native void SetPlayer(DayZPlayer player);
//proto native void SetPlayerType(string type);
proto native DayZPlayer GetDummyPlayer();
proto native void Refresh();
proto native void SetModelOrientation(vector vOrientation);
proto native vector GetModelOrientation();
proto native void SetModelPosition(vector vPos);
proto native vector GetModelPosition();
};
//-----------------------------------------------------------------------------
class HtmlWidget extends RichTextWidget
{
proto native void LoadFile(string path);
};
//-----------------------------------------------------------------------------
class MapWidget: Widget
{
proto native void ClearUserMarks();
proto native void AddUserMark(vector pos, string text, int color /*ARGB*/, string texturePath);
proto native vector GetMapPos();
proto native void SetMapPos(vector worldPos);
proto native float GetScale();
proto native void SetScale(float scale);
proto native float GetContourInterval();
proto native float GetCellSize(float pLegendWidth);
proto native vector MapToScreen(vector worldPos);
proto native vector ScreenToMap(vector screenPos);
};
//-----------------------------------------------------------------------------
class PlayerIdentityBase : Managed
{
proto int GetPingAct();
proto int GetPingMin();
proto int GetPingMax();
proto int GetPingAvg();
proto int GetBandwidthMin();
proto int GetBandwidthMax();
proto int GetBandwidthAvg();
proto float GetOutputThrottle();
proto float GetInputThrottle();
proto string GetName();
proto string GetPlainName();
proto string GetFullName();
proto string GetId();
proto string GetPlainId();
proto int GetPlayerId();
proto Man GetPlayer();
#ifdef FEATURE_NETWORK_RECONCILIATION
proto native void Possess(Pawn pawn);
#endif
private void PlayerIdentityBase();
private void ~PlayerIdentityBase();
};
class PlayerIdentity : PlayerIdentityBase
{
}
//-----------------------------------------------------------------------------
const int PROGRESS_START = 0;
const int PROGRESS_FINISH = 1;
const int PROGRESS_PROGRESS = 2;
const int PROGRESS_UPDATE = 3;
//-----------------------------------------------------------------------------
typedef int ChatChannel;
//-----------------------------------------------------------------------------
typedef Param3<int, float, string> ProgressEventParams;
typedef Param1<string> ScriptLogEventParams;
typedef Param4<int, string, string, string> ChatMessageEventParams;
typedef Param1<int> ChatChannelEventParams;
typedef Param1<int> SQFConsoleEventParams;
typedef Param2<string, string> ClientConnectedEventParams;
typedef Param5<PlayerIdentity, bool, vector, float, int> ClientPrepareEventParams;
typedef Param3<PlayerIdentity, vector, Serializer> ClientNewEventParams;
typedef Param2<PlayerIdentity, Man> ClientNewReadyEventParams;
typedef Param2<PlayerIdentity, Man> ClientRespawnEventParams;
typedef Param2<PlayerIdentity, Man> ClientReadyEventParams;
typedef Param2<PlayerIdentity, Man> ClientReconnectEventParams;
typedef Param4<PlayerIdentity, Man, int, bool> ClientDisconnectedEventParams;
typedef Param1<int> LoginTimeEventParams;
typedef Param1<int> RespawnEventParams;
typedef Param1<vector> PreloadEventParams;
typedef Param1<Man> LogoutCancelEventParams;
typedef Param1<PlayerIdentity> ConnectivityStatsUpdatedEventParams;
typedef Param4<float, float, int, int> ServerFpsStatsUpdatedEventParams;
typedef Param2<string, string> LoginStatusEventParams;
typedef Param1<int> LogoutEventParams;
typedef Param3<int, int, bool> WindowsResizeEventParams;
typedef Param2<bool, bool> VONStateEventParams;
typedef Param2<string, string> VONStartSpeakingEventParams;
typedef Param2<string, string> VONStopSpeakingEventParams;
typedef Param1<string> DLCOwnerShipFailedParams;
typedef Param1<FreeDebugCamera> SetFreeCameraEventParams;
typedef Param1<int> MPConnectionLostEventParams;
typedef Param2<int, string> MPConnectionCloseEventParams;
typedef Param2<DayZPlayer, Object> PlayerDeathEventParams;
typedef Param1<bool> NetworkInputBufferEventParams;
//-----------------------------------------------------------------------------
const EventType StartupEventTypeID;
const EventType WorldCleaupEventTypeID;
//-----------------------------------------------------------------------------
const EventType MPSessionStartEventTypeID;
const EventType MPSessionEndEventTypeID;
const EventType MPSessionFailEventTypeID;
const EventType MPSessionPlayerReadyEventTypeID;
const EventType MPConnectionLostEventTypeID;
const EventType MPConnectionCloseEventTypeID;
//-----------------------------------------------------------------------------
const EventType ProgressEventTypeID;
const EventType NetworkManagerClientEventTypeID;
const EventType NetworkManagerServerEventTypeID;
const EventType DialogQueuedEventTypeID;
//-----------------------------------------------------------------------------
const EventType ChatMessageEventTypeID;
const EventType ChatChannelEventTypeID;
//-----------------------------------------------------------------------------
const EventType ClientConnectedEventTypeID;
const EventType ClientPrepareEventTypeID;
const EventType ClientNewEventTypeID;
const EventType ClientNewReadyEventTypeID;
const EventType ClientRespawnEventTypeID;
const EventType ClientReconnectEventTypeID;
const EventType ClientReadyEventTypeID;
const EventType ClientDisconnectedEventTypeID;
const EventType ClientRemovedEventTypeID;
//-----------------------------------------------------------------------------
const EventType ConnectivityStatsUpdatedEventTypeID;
const EventType ServerFpsStatsUpdatedEventTypeID;
const EventType LogoutCancelEventTypeID;
const EventType LoginTimeEventTypeID;
const EventType RespawnEventTypeID;
const EventType PreloadEventTypeID;
const EventType LogoutEventTypeID;
const EventType LoginStatusEventTypeID;
//-----------------------------------------------------------------------------
const EventType SelectedUserChangedEventTypeID;
const EventType ScriptLogEventTypeID;
//-----------------------------------------------------------------------------
const EventType VONStateEventTypeID;
const EventType VONStartSpeakingEventTypeID;
const EventType VONStopSpeakingEventTypeID;
const EventType VONUserStartedTransmittingAudioEventTypeID;
const EventType VONUserStoppedTransmittingAudioEventTypeID;
//-----------------------------------------------------------------------------
const EventType PartyChatStatusChangedEventTypeID;
const EventType DLCOwnerShipFailedEventTypeID;
const EventType SetFreeCameraEventTypeID;
const EventType ConnectingStartEventTypeID;
const EventType ConnectingAbortEventTypeID;
const EventType PlayerDeathEventTypeID;
const EventType NetworkInputBufferEventTypeID;
//possible in engine events not accessable from script
//ReloadShadersEvent
//LoadWorldProgressEvent
//SignStatusEvent
//SetPausedEvent
//TerminationEvent
//UserSettingsChangedEvent
//StorageChangedEvent
//BeforeResetEvent
//AfterRenderEvent
//AfterResetEvent
//CrashLogEvent
//ConsoleEvent
class CursorIcons
{
const string None = "";
const string Cursor = "set:dayz_gui image:cursor";
const string CloseDoors = "set:dayz_gui image:close";
const string OpenDoors = "set:dayz_gui image:open";
const string OpenCarDoors = "set:dayz_gui image:open_car";
const string CloseCarDoors = "set:dayz_gui image:close_car";
const string EngineOff = "set:dayz_gui image:engine_off";
const string EngineOn = "set:dayz_gui image:engine_on";
const string LadderDown = "set:dayz_gui image:ladderdown";
const string LadderOff = "set:dayz_gui image:ladderoff";
const string LadderUp = "set:dayz_gui image:ladderup";
const string LootCorpse = "set:dayz_gui image:gear";
const string CloseHood = "set:dayz_gui image:close_hood";
const string OpenHood = "set:dayz_gui image:open_hood";
const string GetOut = "set:dayz_gui image:getout";
const string GetInCargo = "set:dayz_gui image:get_in_cargo";
const string Reload = "set:dayz_gui image:reload";
const string GetInDriver = "set:dayz_gui image:get_in_driver";
const string GetInCommander = "set:dayz_gui image:get_in_commander";
const string GetInPilot = "set:dayz_gui image:get_in_pilot";
const string GetInGunner = "set:dayz_gui image:get_in_gunner";
};
// some defs for CGame::ShowDialog()
/*
const int DBB_NONE = 0;
const int DBB_OK = 1;
const int DBB_YES = 2;
const int DBB_NO = 3;
const int DBB_CANCEL = 4;
const int DBT_OK = 0; //just OK button
const int DBT_YESNO = 1; //Yes and No buttons
const int DBT_YESNOCANCEL = 2; //Yes, No, Cancel buttons
const int DMT_NONE = 0;
const int DMT_INFO = 1;
const int DMT_WARNING = 2;
const int DMT_QUESTION = 3;
const int DMT_EXCLAMATION = 4;
*/
DayZGame GetGame() { return g_Game; }
class Hud : Managed
{
ref Timer m_Timer;
void Init(Widget hud_panel_widget) {}
void DisplayNotifier(int key, int tendency, int status) {}
void DisplayBadge(int key, int value) {}
void SetStamina(int value, int range) {}
void DisplayStance(int stance) {}
void DisplayPresence() {}
void ShowCursor() { }
void HideCursor() { }
void SetCursorIcon(string icon) { }
void SetCursorIconScale(string type, float percentage) { }
void SetCursorIconOffset(string type, float x, float y) { }
void SetCursorIconSize(string type, float x, float y) { }
void ShowWalkieTalkie(bool show) { }
void ShowWalkieTalkie(int fadeOutSeconds) { }
void SetWalkieTalkieText(string text) { }
void RefreshQuickbar(bool itemChanged = false) {}
void Show(bool show) {}
void UpdateBloodName() {}
void SetTemperature(string temp);
void SetStaminaBarVisibility(bool show);
void Update(float timeslice){}
void ShowVehicleInfo();
void HideVehicleInfo();
void InitHeatBufferUI(Man player);
void ShowQuickbarUI(bool show);
void ShowQuickbarPlayer(bool show);
void ShowHudPlayer(bool show);
void ShowHudUI(bool show);
void ShowHudInventory(bool show);
void ShowQuickBar(bool show);
void UpdateQuickbarGlobalVisibility();
void ShowHud(bool show);
void OnResizeScreen();
void OnPlayerLoaded(); // player connects or respawns
void SetPermanentCrossHair(bool show) {}
void SpawnHitDirEffect(DayZPlayer player, float hit_direction,float intensity_max);
void SetConnectivityStatIcon(EConnectivityStatType type, EConnectivityStatLevel level);
};
//-----------------------------------------------------------------------------
class Mission
{
ScriptModule MissionScript;
ref array<vector> m_ActiveRefresherLocations;
protected ref ScriptInvoker m_OnInputDeviceChanged = new ScriptInvoker();
protected ref ScriptInvoker m_OnInputPresetChanged = new ScriptInvoker();
protected ref ScriptInvoker m_OnInputDeviceConnected = new ScriptInvoker();
protected ref ScriptInvoker m_OnInputDeviceDisconnected = new ScriptInvoker();
protected ref ScriptInvoker m_OnModMenuVisibilityChanged = new ScriptInvoker();
protected ref ScriptInvoker m_OnTimeChanged = new ScriptInvoker();
private void ~Mission();
void OnInit() {}
void OnMissionStart() {}
void OnMissionFinish() {}
void OnUpdate(float timeslice)
{
#ifdef FEATURE_CURSOR
m_TimeStamp++;
#endif
}
void OnKeyPress(int key) {}
void OnKeyRelease(int key) {}
void OnMouseButtonPress(int button){}
void OnMouseButtonRelease(int button){}
void OnEvent(EventType eventTypeId, Param params) {}
void OnItemUsed(InventoryItem item, Man owner) {}
void AddDummyPlayerToScheduler(Man player){}
void Reset(){}
void ResetGUI(){}
void OnGameplayDataHandlerLoad(){}
Hud GetHud()
{
return null;
}
ObjectSnapCallback GetInventoryDropCallback()
{
return null;
}
bool IsPlayerDisconnecting(Man player);
UIScriptedMenu CreateScriptedMenu(int id)
{
return null;
}
UIScriptedWindow CreateScriptedWindow(int id)
{
return null;
}
WorldData GetWorldData()
{
return null;
}
WorldLighting GetWorldLighting()
{
return null;
}
DynamicMusicPlayer GetDynamicMusicPlayer()
{
return null;
}
bool IsPaused()
{
return false;
}
bool IsGame()
{
return false;
}
bool IsServer()
{
return false;
}
void Pause() {}
void Continue() {}
void AbortMission() {}
void CreateLogoutMenu(UIMenuPanel parent) {}
void StartLogoutMenu(int time) {}
void CreateDebugMonitor() {}
void HideDebugMonitor() {}
void RefreshCrosshairVisibility() {}
void HideCrosshairVisibility() {}
bool IsMissionGameplay()
{
return false;
}
bool IsControlDisabled() {}
int GetControlDisabledMode() {}
void PlayerControlEnable(bool bForceSupress);
void PlayerControlDisable(int mode);
void RemoveActiveInputExcludes(array<string> excludes, bool bForceSupress = false);
void RemoveActiveInputRestriction(int restrictor);
void AddActiveInputExcludes(array<string> excludes);
void AddActiveInputRestriction(int restrictor);
void RefreshExcludes();
bool IsInputExcludeActive(string exclude);
bool IsInputRestrictionActive(int restriction);
void EnableAllInputs(bool bForceSupress = false);
void ShowInventory() {}
void HideInventory() {}
void ShowChat() {}
void HideChat() {}
void UpdateVoiceLevelWidgets(int level) {}
void HideVoiceLevelWidgets() {}
bool IsVoNActive() {}
void SetVoNActive(bool active) {}
bool InsertCorpse(Man player)
{
return false;
}
UIScriptedMenu GetNoteMenu();
void SetNoteMenu(UIScriptedMenu menu);
void SetPlayerRespawning(bool state);
void OnPlayerRespawned(Man player);
bool IsPlayerRespawning();
array<vector> GetActiveRefresherLocations();
void SetRespawnModeClient(int mode);
int GetRespawnModeClient()
{
return -1;
}
void SyncRespawnModeInfo(PlayerIdentity identity);
ImageWidget GetMicrophoneIcon()
{
return null;
}
WidgetFadeTimer GetMicWidgetFadeTimer();
GameplayEffectWidgets_base GetEffectWidgets();
map<int,ImageWidget> GetVoiceLevelWidgets();
map<int,ref WidgetFadeTimer> GetVoiceLevelTimers();
ScriptInvoker GetOnInputDeviceChanged()
{
if (!m_OnInputDeviceChanged)
m_OnInputDeviceChanged = new ScriptInvoker();
return m_OnInputDeviceChanged;
}
ScriptInvoker GetOnInputPresetChanged()
{
if (!m_OnInputPresetChanged)
m_OnInputPresetChanged = new ScriptInvoker();
return m_OnInputPresetChanged;
}
ScriptInvoker GetOnInputDeviceConnected()
{
if (!m_OnInputDeviceConnected)
m_OnInputDeviceConnected = new ScriptInvoker();
return m_OnInputDeviceConnected;
}
ScriptInvoker GetOnInputDeviceDisconnected()
{
if (!m_OnInputDeviceDisconnected)
m_OnInputDeviceDisconnected = new ScriptInvoker();
return m_OnInputDeviceDisconnected;
}
ScriptInvoker GetOnModMenuVisibilityChanged()
{
if (!m_OnModMenuVisibilityChanged)
m_OnModMenuVisibilityChanged = new ScriptInvoker();
return m_OnModMenuVisibilityChanged;
}
ScriptInvoker GetOnTimeChanged()
{
if (!m_OnTimeChanged)
m_OnTimeChanged = new ScriptInvoker();
return m_OnTimeChanged;
}
#ifdef FEATURE_CURSOR
int m_TimeStamp = 0;
int GetTimeStamp()
{
return m_TimeStamp;
}
#endif
#ifdef DEVELOPER
bool m_SuppressNextFrame = true;
void SetInputSuppression(bool state);
bool GetInputSuppression();
#endif
};
// -------------------------------------------------------------------------
class MenuData: Managed
{
proto native int GetCharactersCount();
proto native int GetLastPlayedCharacter();
proto native Man CreateCharacterPerson(int index);
proto void GetLastServerAddress(int index, out string address);
proto native int GetLastServerPort(int index);
proto native int GetLastSteamQueryPort(int index);
proto void GetLastServerName(int index, out string address);
proto void RequestSetDefaultCharacterData();
proto bool RequestGetDefaultCharacterData();
void OnSetDefaultCharacter(ParamsWriteContext ctx)
{
if (!GetMenuDefaultCharacterDataInstance()/* || g_Game.IsServer()*/)
{
Error("MenuData | OnSetDefaultCharacter - failed to get data class!");
return;
}
GetMenuDefaultCharacterDataInstance().SerializeCharacterData(ctx);
SaveCharactersLocal();
}
bool OnGetDefaultCharacter(ParamsReadContext ctx)
{
if (!GetMenuDefaultCharacterDataInstance())
{
Error("MenuData | OnGetDefaultCharacter - failed to get data class!");
return false;
}
if (GetMenuDefaultCharacterDataInstance().DeserializeCharacterData(ctx))
{
return true;
}
return false;
}
proto void GetCharacterName(int index, out string name);
proto native void SetCharacterName(int index, string newName);
// save character is set as last played character
proto native void SaveCharacter(bool localPlayer, bool verified);
proto native void SaveDefaultCharacter(Man character);
proto native void SaveCharactersLocal();
proto native void LoadCharactersLocal();
proto native void ClearCharacters();
MenuDefaultCharacterData GetMenuDefaultCharacterDataInstance()
{
return g_Game.GetMenuDefaultCharacterData();
}
//proto native void GetCharacterStringList(int characterID, string name, out TStringArray values);
//proto bool GetCharacterString(int characterID,string name, out string value);
};
//holds &#39;default character&#39; data
class MenuDefaultCharacterData
{
//const int MODE_SERVER = 0;
//const int MODE_CLIENT = 1;
string m_CharacterName;
string m_CharacterType;
ref map<int,string> m_AttachmentsMap;
bool m_ForceRandomCharacter;
void MenuDefaultCharacterData()
{
Init();
}
void Init()
{
if (!g_Game.IsDedicatedServer())
{
g_Game.GetMenuData().LoadCharactersLocal();
}
m_AttachmentsMap = new map<int,string>;
}
void ClearAttachmentsMap()
{
m_AttachmentsMap.Clear();
}
void SetDefaultAttachment(int slotID, string type)
{
m_AttachmentsMap.Set(slotID,type); //using &#39;Set&#39; instead of &#39;Insert&#39; for convenience
}
void GenerateRandomEquip()
{
ClearAttachmentsMap();
int slot_ID;
string attachment_type;
for (int i = 0; i < DefaultCharacterCreationMethods.GetAttachmentSlotsArray().Count(); i++)
{
slot_ID = DefaultCharacterCreationMethods.GetAttachmentSlotsArray().Get(i);
if (DefaultCharacterCreationMethods.GetConfigArrayCountFromSlotID(slot_ID) > 0)
{
attachment_type = DefaultCharacterCreationMethods.GetConfigAttachmentTypes(slot_ID).GetRandomElement();
//if (attachment_type != "")
SetDefaultAttachment(slot_ID,attachment_type);
}
}
}
void EquipDefaultCharacter(Man player)
{
if (!player)
{
ErrorEx("WARNING - trying to equip non-existent object! | MenuDefaultCharacterData::EquipDefaultCharacter");
return;
}
int slot_ID;
string attachment_type;
string current_attachment_type;
EntityAI current_attachment_object;
GameInventory playerInventory = player.GetInventory();
for (int i = 0; i < m_AttachmentsMap.Count(); ++i)
{
attachment_type = "";
current_attachment_type = "";
slot_ID = m_AttachmentsMap.GetKey(i);
attachment_type = m_AttachmentsMap.GetElement(i); //Get(i)
current_attachment_object = playerInventory.FindAttachment(slot_ID);
if (current_attachment_object)
{
current_attachment_type = current_attachment_object.GetType();
}
if (current_attachment_type != attachment_type)
{
if (current_attachment_object)
g_Game.ObjectDelete(current_attachment_object);
if (attachment_type != "")
playerInventory.CreateAttachmentEx(attachment_type,slot_ID);
}
}
}
void SerializeCharacterData(ParamsWriteContext ctx)
{
ctx.Write(m_CharacterType);
ctx.Write(m_AttachmentsMap);
ctx.Write(m_ForceRandomCharacter);
ctx.Write(m_CharacterName);
//DumpAttMapContents();
}
bool DeserializeCharacterData(ParamsReadContext ctx)
{
if (!ctx.Read(m_CharacterType))
return false;
if (!ctx.Read(m_AttachmentsMap))
return false;
if (!ctx.Read(m_ForceRandomCharacter))
return false;
if (!ctx.Read(m_CharacterName))
return false;
//DumpAttMapContents();
return true;
}
void SetCharacterName(string name)
{
m_CharacterName = name;
}
string GetCharacterName()
{
return m_CharacterName;
}
void SetCharacterType(string character_type)
{
m_CharacterType = character_type;
}
string GetCharacterType()
{
return m_CharacterType;
}
void SetRandomCharacterForced(bool state)
{
m_ForceRandomCharacter = state;
}
bool IsRandomCharacterForced()
{
return m_ForceRandomCharacter;
}
map<int,string> GetAttachmentMap()
{
return m_AttachmentsMap;
}
//DEBUG
void DumpAttMapContents()
{
int debugID;
string debugType;
Print("-----------");
Print("m_AttachmentsMap contents:");
for (int j = 0; j < m_AttachmentsMap.Count(); j++)
{
debugID = m_AttachmentsMap.GetKey(j);
debugType = m_AttachmentsMap.GetElement(j);
Print("index " + j);
Print("debugID: " + debugID);
Print("debugType: " + debugType);
}
Print("-----------");
}
}
class DefaultCharacterCreationMethods
{
static ref array<int> m_AttachmentSlots = {
InventorySlots.SHOULDER,
InventorySlots.MELEE,
InventorySlots.HEADGEAR,
InventorySlots.MASK,
InventorySlots.EYEWEAR,
InventorySlots.GLOVES,
InventorySlots.ARMBAND,
InventorySlots.BODY,
InventorySlots.VEST,
InventorySlots.BACK,
InventorySlots.HIPS,
InventorySlots.LEGS,
InventorySlots.FEET
};
//conversion nescesssary for legacy reasons...
static ref array<string> m_ConfigArrayNames = {
"shoulder",
"melee",
"headgear",
"mask",
"eyewear",
"gloves",
"armband",
"top",
"vests",
"backpacks",
"hips",
"bottom",
"shoe"
};
const static string m_Path = "cfgCharacterCreation";
static string GetPathFromSlotID(int slot_ID)
{
int idx = m_AttachmentSlots.Find(slot_ID);
string path = "" + m_Path + " " + m_ConfigArrayNames.Get(idx);
return path;
}
static int GetConfigArrayCountFromSlotID(int slot_ID)
{
TStringArray types = new TStringArray;
g_Game.ConfigGetTextArray(GetPathFromSlotID(slot_ID),types);
return types.Count();
}
static array<string> GetConfigAttachmentTypes(int slot_ID)
{
TStringArray types = new TStringArray;
g_Game.ConfigGetTextArray(GetPathFromSlotID(slot_ID),types);
return types;
}
static array<int> GetAttachmentSlotsArray()
{
return m_AttachmentSlots;
}
static array<string> GetConfigArrayNames()
{
return m_ConfigArrayNames;
}
}
enum OptionAccessType
{
AT_UNKNOWN,
AT_OBJECTS_DETAIL,
AT_TEXTURE_DETAIL,
AT_HDR_DETAIL,
AT_FSAA_DETAIL,
AT_VSYNC_VALUE,
AT_ANISO_DETAIL,
AT_OPTIONS_FXAA_VALUE,
AT_OPTIONS_SW_VALUE,
AT_POSTPROCESS_EFFECTS,
AT_QUALITY_PREFERENCE,
AT_ATOC_DETAIL,
AT_AMBIENT_OCCLUSION,
AT_BLOOM,
AT_ROTATION_BLUR,
AT_SHADOW_DETAIL,
AT_OPTIONS_TERRAIN,
AT_OPTIONS_RESOLUTION,
AT_OPTIONS_GAMMA_SLIDER,
AT_OPTIONS_BRIGHT_SLIDER,
AT_OPTIONS_VISIBILITY_SLIDER,
AT_OPTIONS_OBJECT_VISIBILITY_SLIDER,
AT_OPTIONS_SHADOW_VISIBILITY_SLIDER,
AT_OPTIONS_DRAWDISTANCE_SLIDER,
AT_ASPECT_RATIO,
AT_OPTIONS_FIELD_OF_VIEW,
AT_OPTIONS_MUSIC_SLIDER,
AT_OPTIONS_EFFECTS_SLIDER,
AT_OPTIONS_VON_SLIDER,
AT_OPTIONS_MASTER_VOLUME,
AT_OPTIONS_HWACC,
AT_OPTIONS_EAX,
AT_OPTIONS_LANGUAGE,
AT_OPTIONS_RADIO,
AT_CONFIG_MOUSE_FILTERING,
AT_CONFIG_HEAD_BOB,
AT_OPTIONS_DISPLAY_MODE,
AT_OPTIONS_TERRAIN_SHADER,
AT_OPTIONS_AIM_HELPER,
AT_OPTIONS_MOUSE_AND_KEYBOARD,
AT_OPTIONS_PAUSE,
AT_OPTIONS_FLIPMODE,
AT_OPTIONS_VON_THRESHOLD_SLIDER,
AT_OPTIONS_MOUSE_YAXIS_INVERTED,
AT_OPTIONS_MOUSE_XAXIS,
AT_OPTIONS_MOUSE_YAXIS,
AT_OPTIONS_MOUSE_XAXIS_AIM_MOD,
AT_OPTIONS_MOUSE_YAXIS_AIM_MOD,
AT_OPTIONS_CONTROLLER_LS_XAXIS,
AT_OPTIONS_CONTROLLER_LS_YAXIS,
AT_OPTIONS_CONTROLLER_LS_XAXIS_VEHICLE_MOD,
AT_OPTIONS_CONTROLLER_RS_YAXIS_INVERTED,
AT_OPTIONS_CONTROLLER_RS_XAXIS,
AT_OPTIONS_CONTROLLER_RS_YAXIS,
AT_OPTIONS_CONTROLLER_RS_XAXIS_AIM_MOD,
AT_OPTIONS_CONTROLLER_RS_YAXIS_AIM_MOD,
AT_OPTIONS_VON_INPUT_MODE,
AT_OPTIONS_CONTROLLER_LS_DEADZONE,
AT_OPTIONS_CONTROLLER_RS_DEADZONE,
AT_OPTIONS_VISIBILITY_COMBO,
AT_OPTIONS_OBJECT_VISIBILITY_COMBO,
};
enum OptionIDsScript
{
OPTION_HUD = 100, //starts at 100 to avoid ID conflict with AT_
OPTION_HUD_VEHICLE,
OPTION_CROSSHAIR,
OPTION_GAME_MESSAGES,
OPTION_ADMIN_MESSAGES,
OPTION_PLAYER_MESSAGES,
OPTION_QUICKBAR,
OPTION_SERVER_INFO,
OPTION_BLEEDINGINDICATION,
OPTION_CONNECTIVITY_INFO,
OPTION_HUD_BRIGHTNESS,
OPTION_AMBIENT_MUSIC_MODE
};
// -------------------------------------------------------------------------
/*
// Option Access Control Type
const int OA_CT_NUMERIC = 0;
const int OA_CT_SWITCH = 1;
const int OA_CT_LIST = 2;
// Option Field of view constants
const float OPTIONS_FIELD_OF_VIEW_MIN = 0.75242724772f;
const float OPTIONS_FIELD_OF_VIEW_MAX = 1.30322025726f;
*/
class OptionsAccess: Managed
{
//proto private void ~OptionsAccess();
//proto private void OptionsAccess();
proto native int GetAccessType();
proto native int GetControlType();
proto native void Apply();
proto native void Test();
proto native void Revert();
proto native int IsChanged();
proto native int NeedRestart();
proto native int SetChangeImmediately();
//proto native void Initialize();
private proto void SetScriptEvents(Managed events);
private proto Managed GetScriptEvents();
OptionsAccessEvents GetEvents()
{
return OptionsAccessEvents.Cast(GetScriptEvents());
}
void OnRevert()
{
GetEvents().Event_OnRevert.Invoke(this);
}
};
class OptionsAccessEvents
{
ref ScriptInvoker Event_OnRevert = new ScriptInvoker();
}
// -------------------------------------------------------------------------
class NumericOptionsAccess extends OptionsAccess
{
proto native float ReadValue();
proto native void WriteValue(float value);
proto native float GetMin();
proto native float GetMax();
proto native float GetDefault();
};
// -------------------------------------------------------------------------
class ListOptionsAccess extends OptionsAccess
{
proto native int GetIndex();
proto native int GetDefaultIndex();
proto native void SetIndex(int index);
proto native int GetItemsCount();
proto void GetItemText(int index, out string value);
void GetAllItemsText(array<string> output)
{
string s;
for (int i = 0; i < GetItemsCount(); ++i)
{
GetItemText(i, s);
output.Insert(s);
}
}
};
// -------------------------------------------------------------------------
class SwitchOptionsAccess extends OptionsAccess
{
proto native void Switch();
proto void GetItemText(out string value);
proto native int GetIndex();
proto native int GetDefaultIndex();
};
// -------------------------------------------------------------------------
class GameOptions: Managed
{
proto native void Apply();
proto native void Revert();
proto native void Test();
proto native OptionsAccess GetOption(int index);
proto native OptionsAccess GetOptionByType(int accessType);
proto native int GetOptionsCount();
proto native int NeedRestart();
proto native int IsChanged();
proto native void Initialize();
};
typedef Link<Object> OLinkT;
EntityAI SpawnEntity (string object_name, notnull InventoryLocation inv_loc, int iSetupFlags, int iRotation)
{
return GameInventory.LocationCreateEntity(inv_loc, object_name,iSetupFlags,iRotation);
}
class Static : Object
{
};
class PrtTest // Temporal class for toggling particles on guns
{
static bool m_GunParticlesState = true;
}
// #include "Scripts/Classes/Component/_include.c"