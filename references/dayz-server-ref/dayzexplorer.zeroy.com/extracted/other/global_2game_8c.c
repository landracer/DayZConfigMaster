// ======================================================================
// File: global_2game_8c_source.html
// Category: other
// ======================================================================

static int GAME_STORAGE_VERSION = 142;
class CGame
{
// enableDebugMonitor in server config
int m_DebugMonitorEnabled;
ScriptModule GameScript;
//Obsolete, port [Obsolete()] as well, maybe?
private ref array<ref Param> m_ParamCache;
//analytics
ref AnalyticsManagerServer m_AnalyticsManagerServer;
ref AnalyticsManagerClient m_AnalyticsManagerClient;
ref MenuDefaultCharacterData m_CharacterData;
#ifdef DIAG_DEVELOPER
ref array<ComponentEnergyManager> m_EnergyManagerArray;
void EnableEMPlugs(bool enable)
{
for (int i = 0; i < g_Game.m_EnergyManagerArray.Count(); ++i)
{
if (g_Game.m_EnergyManagerArray[i])
g_Game.m_EnergyManagerArray[i].SetDebugPlugs(enable);
}
}
#endif
void CGame()
{
Math.Randomize(-1);
LogManager.Init();
m_ParamCache = new array<ref Param>;
m_ParamCache.Insert(null);
//analytics
m_AnalyticsManagerServer = new AnalyticsManagerServer;
m_AnalyticsManagerClient = new AnalyticsManagerClient;
//m_CharacterData = new MenuCharacrerData;
// actual script version - increase by one when you make changes
StorageVersion(GAME_STORAGE_VERSION);
#ifdef DIAG_DEVELOPER
m_EnergyManagerArray = new array<ComponentEnergyManager>;
#endif
if (!IsDedicatedServer())
{
SEffectManager.Init();
AmmoEffects.Init();
VONManager.Init();
if (!IsMultiplayer())
{
SEffectManager.InitServer();
}
}
else
{
SEffectManager.InitServer();
}
}
private void ~CGame()
{
// Clean these up even if it is dedicated server, just to be safe
SEffectManager.Cleanup();
AmmoEffects.Cleanup();
VONManager.CleanupInstance();
// Is initialized in StartupEvent
ParticleManager.CleanupInstance();
g_Game = null;
}
proto native WorkspaceWidget GetWorkspace();
proto native WorkspaceWidget GetLoadingWorkspace();
void OnEvent(EventType eventTypeId, Param params)
{
}
//PLM Type: 0 == RESUMED, 1 == SUSPENDED
void OnProcessLifetimeChanged(int plmtype)
{
}
void OnLicenseChanged()
{
}
void OnAfterCreate()
{
}
void OnActivateMessage()
{
}
void OnDeactivateMessage()
{
}
bool OnInitialize()
{
return false;
}
void OnDeviceReset()
{
}
void OnUpdate(bool doSim, float timeslice)
{
}
void OnPostUpdate(bool doSim, float timeslice)
{
}
void OnKeyPress(int key)
{
}
void OnKeyRelease(int key)
{
}
void OnMouseButtonPress(int button)
{
}
void OnMouseButtonRelease(int button)
{
}
UIScriptedMenu CreateScriptedMenu( int id ) { }
UIScriptedWindow CreateScriptedWindow( int id ) { }
void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
{
}
proto native void RequestExit( int code );
proto native void RequestRestart(int code);
proto native bool IsAppActive();
proto bool GetHostAddress( out string address, out int port );
proto owned string GetHostName();
proto GetServersResultRow GetHostData();
proto native int Connect( UIScriptedMenu parent , string IpAddress, int port, string password );
proto native int ConnectLastSession( UIScriptedMenu parent , int selectedCharacter = -1 );
proto native void DisconnectSession();
proto native void DisconnectSessionForce();
// profile functions
proto native void GetProfileStringList(string name, out TStringArray values);
proto bool GetProfileString(string name, out string value);
proto native void SetProfileStringList(string name, TStringArray values);
proto native void SetProfileString(string name, string value);
proto native void SaveProfile();
proto void GetPlayerName(out string name);
proto void GetPlayerNameShort(int maxLength, out string name);
proto native void SetPlayerName(string name);
proto native Entity CreatePlayer(PlayerIdentity identity, string name, vector pos, float radius, string spec);
proto native void SelectPlayer(PlayerIdentity identity, Object player);
proto void GetPlayerNetworkIDByIdentityID( int playerIdentityID, out int networkIdLowBits, out int networkIdHightBits );
proto native Object GetObjectByNetworkId( int networkIdLowBits, int networkIdHighBits );
proto native bool RegisterNetworkStaticObject(Object object);
proto native bool IsNetworkInputBufferFull();
proto native void SelectSpectator(PlayerIdentity identity, string spectatorObjType, vector position);
proto native void UpdateSpectatorPosition(vector position);
proto native void SendLogoutTime(Object player, int time);
proto native void DisconnectPlayer(PlayerIdentity identity, string uid = "");
proto native void AddToReconnectCache(PlayerIdentity identity);
proto native void RemoveFromReconnectCache(string uid);
proto native void ClearReconnectCache();
proto native void StorageVersion( int iVersion );
proto native int LoadVersion();
proto native int SaveVersion();
proto native float GetDayTime();
// config functions
proto bool ConfigGetText(string path, out string value);
proto bool ConfigGetTextRaw(string path, out string value);
string ConfigGetTextOut(string path)
{
string ret_s;
ConfigGetText(path, ret_s);
return ret_s;
}
bool FormatRawConfigStringKeys(inout string value)
{
int ret;
ret = value.Replace("$STR_","#STR_");
return ret > 0;
}
string GetModelName(string class_name)
{
if ( class_name != "" )
{
string cfg = "CfgVehicles " + class_name + " model";
string model_path;
if ( g_Game.ConfigGetText(cfg, model_path) )
{
int to_substring_end = model_path.Length() - 4; // -4 to leave out the &#39;.p3d&#39; suffix
int to_substring_start = 0;
// Currently we have model path. To get the name out of it we need to parse this string from the end and stop at the first found &#39;\&#39; sign
for (int i = to_substring_end; i > 0; i--)
{
string sign = model_path.Get(i);
if ( sign == "\\" )
{
to_substring_start = i + 1;
break
}
}
string model_name = model_path.Substring(to_substring_start, to_substring_end - to_substring_start);
return model_name;
}
}
return "UNKNOWN_P3D_FILE";
}
proto native float ConfigGetFloat(string path);
proto native vector ConfigGetVector(string path);
proto native int ConfigGetInt(string path);
proto native int ConfigGetType(string path);
proto native void ConfigGetTextArray(string path, out TStringArray values);
proto native void ConfigGetTextArrayRaw(string path, out TStringArray values);
proto native void ConfigGetFloatArray(string path, out TFloatArray values);
proto native void ConfigGetIntArray(string path, out TIntArray values);
proto bool ConfigGetChildName(string path, int index, out string name);
proto bool ConfigGetBaseName(string path, out string base_name);
proto native int ConfigGetChildrenCount(string path);
proto native bool ConfigIsExisting(string path);
proto native void ConfigGetFullPath(string path, out TStringArray full_path);
proto native void ConfigGetObjectFullPath(Object obj, out TStringArray full_path);
proto native void GetModInfos(notnull out array<ref ModInfo> modArray);
proto native bool GetModToBeReported();
string ConfigPathToString(TStringArray array_path)
{
string return_path = "";
int count = array_path.Count();
for (int i = 0; i < count; i++)
{
return_path += array_path.Get(i);
if ( i < count - 1 )
{
return_path += " ";
}
}
return return_path;
}
proto bool CommandlineGetParam(string name, out string value);
proto native void CopyToClipboard(string text);
proto void CopyFromClipboard(out string text);
proto native void BeginOptionsVideo();
proto native void EndOptionsVideo();
proto native void AdminLog(string text);
// entity functions
proto native bool PreloadObject( string type, float distance );
proto native Object CreateStaticObjectUsingP3D(string p3dFilename, vector position, vector orientation, float scale = 1.0, bool createLocal = false);
proto native Object CreateObject( string type, vector pos, bool create_local = false, bool init_ai = false, bool create_physics = true );
proto native SoundOnVehicle CreateSoundOnObject(Object source, string sound_name, float distance, bool looped, bool create_local = false);
proto native SoundWaveOnVehicle CreateSoundWaveOnObject(Object source, SoundObject soundObject, AbstractWave soundWave);
proto native Object CreateObjectEx( string type, vector pos, int iFlags, int iRotation = RF_DEFAULT );
proto native void ObjectDelete( Object obj );
proto native void ObjectDeleteOnClient( Object obj );
proto native void RemoteObjectDelete( Object obj );
proto native void RemoteObjectTreeDelete( Object obj );
proto native void RemoteObjectCreate( Object obj );
proto native void RemoteObjectTreeCreate( Object obj );
proto native int ObjectRelease( Object obj );
proto void ObjectGetType( Object obj, out string type );
proto void ObjectGetDisplayName( Object obj, out string name );
proto native vector ObjectGetSelectionPosition(Object obj, string name);
proto native vector ObjectGetSelectionPositionLS(Object obj, string name);
proto native vector ObjectGetSelectionPositionMS(Object obj, string name);
proto native vector ObjectGetSelectionPositionWS(Object obj, string name);
proto native vector ObjectModelToWorld(Object obj, vector modelPos);
proto native vector ObjectWorldToModel(Object obj, vector worldPos);
proto native bool IsObjectAccesible(EntityAI item, Man player);
#ifdef DIAG_DEVELOPER
proto native void ReloadShape(Object obj);
#endif
// input
proto native Input GetInput();
// camera
proto native vector GetCurrentCameraPosition();
proto native vector GetCurrentCameraDirection();
// sound
proto native AbstractSoundScene GetSoundScene();
// noise
proto native NoiseSystem GetNoiseSystem();
// inventory
proto native bool AddInventoryJuncture(Man player, notnull EntityAI item, InventoryLocation dst, bool test_dst_occupancy, int timeout_ms, Managed userData = null);
bool AddInventoryJunctureEx(Man player, notnull EntityAI item, InventoryLocation dst, bool test_dst_occupancy, int timeout_ms, Managed userData = null)
{
bool result = AddInventoryJuncture(player, item, dst, test_dst_occupancy, timeout_ms/*10000000*/, userData);
#ifdef ENABLE_LOGGING
if ( LogManager.IsInventoryReservationLogEnable() )
{
Debug.InventoryReservationLog("STS = " + player.GetSimulationTimeStamp() + " result: " + result + " item:" + item + " dst: " + InventoryLocation.DumpToStringNullSafe(dst), "n/a" , "n/a", "AddInventoryJuncture",player.ToString() );
}
#endif
//Print("Juncture - STS = " + player.GetSimulationTimeStamp() + " item:" + item + " dst: " + InventoryLocation.DumpToStringNullSafe(dst));
return result;
}
//Has inventory juncture for any player
proto native bool HasInventoryJunctureItem(notnull EntityAI item);
proto native bool HasInventoryJuncture(Man player, notnull EntityAI item);
proto native bool HasInventoryJunctureDestination(Man player, notnull InventoryLocation dst);
proto native bool AddActionJuncture(Man player, notnull EntityAI item, int timeout_ms, Managed userData = null);
proto native bool ExtendActionJuncture(Man player, notnull EntityAI item, int timeout_ms);
proto native bool ClearJuncture(Man player, notnull EntityAI item);
bool ClearJunctureEx(Man player, notnull EntityAI item)
{
#ifdef ENABLE_LOGGING
if ( LogManager.IsInventoryReservationLogEnable() )
{
Debug.InventoryReservationLog("STS = " + player.GetSimulationTimeStamp()+ " item:" + item, "n/a" , "n/a", "ClearJuncture",player.ToString() );
}
#endif
return ClearJuncture( player, item);
}
// support
proto native bool ExecuteEnforceScript(string expression, string mainFnName);
proto native void DumpInstances(bool csvFormatting);
proto native bool ScriptTest();
proto native void GetDiagModeNames(out TStringArray diag_names);
proto native void SetDiagModeEnable(int diag_mode, bool enabled);
proto native bool GetDiagModeEnable(int diag_mode);
proto native void GetDiagDrawModeNames(out TStringArray diag_names);
proto native void SetDiagDrawMode(int diag_draw_mode);
proto native int GetDiagDrawMode();
proto native bool IsPhysicsExtrapolationEnabled();
[Obsolete("Use GizmoApi.GetCount")]
proto native int GizmoGetCount();
[Obsolete("Use GizmoApi.GetInstance")]
proto native Class GizmoGetInstance(int index);
[Obsolete("Use GizmoApi.GetTracker")]
proto native Managed GizmoGetTracker(int index);
[Obsolete("Use GizmoApi.FindByTracker")]
proto native int GizmoFindByTracker(Managed tracker);
[Obsolete("Use GizmoApi.Deselect")]
proto native void GizmoClear(int index);
[Obsolete("Use GizmoApi.DeselectAll")]
proto native void GizmoClearAll();
[Obsolete("Use GizmoApi.SelectObject")]
proto native void GizmoSelectObject(Object object);
[Obsolete("Use GizmoApi.SelectPhysics")]
proto native void GizmoSelectPhysics(Physics physics);
[Obsolete("Use GizmoApi.SelectUser")]
proto native void GizmoSelectUser(Managed instance);
proto native float GetFps();
proto native float GetLastFPS();
proto native float GetAvgFPS(int nFrames = 64);
proto native float GetMinFPS(int nFrames = 64);
proto native float GetMaxFPS(int nFrames = 64);
void GetFPSStats(out float min, out float max, out float avg, int nFrames = 64)
{
min = GetMinFPS(nFrames);
max = GetMaxFPS(nFrames);
avg = GetAvgFPS(nFrames);
}
proto native float GetTickTime();
proto void GetInventoryItemSize(InventoryItem item, out int width, out int height);
proto native void GetObjectsAtPosition(vector pos, float radius, out array<Object> objects, out array<CargoBase> proxyCargos);
proto native void GetObjectsAtPosition3D(vector pos, float radius, out array<Object> objects, out array<CargoBase> proxyCargos);
proto native World GetWorld();
proto void GetWorldName( out string world_name );
string GetWorldName()
{
string world_name;
g_Game.GetWorldName(world_name);
return world_name;
}
proto native bool VerifyWorldOwnership( string sWorldName );
proto native bool GoBuyWorldDLC( string sWorldName );
proto void FormatString( string format, string params[], out string output);
proto void GetVersion( out string version );
proto native UIManager GetUIManager();
proto native DayZPlayer GetPlayer();
proto native void GetPlayers( out array<Man> players );
DayZPlayer GetPlayerByIndex(int index = 0)
{
array<Man> players();
GetPlayers(players);
if (index >= players.Count())
return null;
return DayZPlayer.Cast(players[index]);
}
proto native void StoreLoginData(ParamsWriteContext ctx);
proto native vector GetPointerDirection();
proto native vector GetWorldDirectionFromScreen(vector world_pos);
proto native vector GetScreenPos(vector world_pos);
proto native vector GetScreenPosRelative(vector world_pos);
proto native MenuData GetMenuData();
proto native void RPC(Object target, int rpcType, notnull array<ref Param> params, bool guaranteed,PlayerIdentity recipient = null);
proto native void RPCSingleParam(Object target, int rpc_type, Param param, bool guaranteed, PlayerIdentity recipient = null);
proto native void RPCSelf(Object target, int rpcType, notnull array<ref Param> params);
proto native void RPCSelfSingleParam(Object target, int rpcType, Param param);
proto native void ProfilerStart(string name);
proto native void ProfilerStop(string name);
proto native void Chat(string text, string colorClass);
proto native void ChatMP(Man recipient, string text, string colorClass);
proto native void ChatPlayer(string text);
proto native void MutePlayer(string muteUID, string playerUID, bool mute);
proto native void MuteAllPlayers(string listenerId, bool mute);
proto native void EnableVoN(Object player, bool enable);
proto native void SetVoiceEffect(Object player, int effect, bool enable);
proto native void SetVoiceLevel(int level);
proto native int GetVoiceLevel(Object player = null);
proto native void EnableMicCapture(bool enable);
proto native bool IsMicCapturing();
proto native bool IsInPartyChat();
// mission
proto native Mission GetMission();
proto native void SetMission(Mission mission);
proto native void StartRandomCutscene(string world);
proto native void PlayMission(string path);
proto protected native void CreateMission(string path);
proto native void RestartMission();
proto native void AbortMission();
proto native void RespawnPlayer();
proto native bool CanRespawnPlayer();
proto native void SetLoginTimerFinished();
proto native void SetMainMenuWorld(string world);
proto native owned string GetMainMenuWorld();
proto native void LogoutRequestTime();
proto native void LogoutRequestCancel();
proto native bool IsMultiplayer();
proto native bool IsClient();
proto native bool IsServer();
proto native bool IsDedicatedServer();
proto native int ServerConfigGetInt(string name);
// Internal build
proto native bool IsDebug();
//#ifdef PLATFORM_XBOX
static bool IsDigitalCopy()
{
return OnlineServices.IsGameActive(false);
}
//#endif
/*bool IsNewMenu()
{
return m_ParamNewMenu;
}*/
void SetDebugMonitorEnabled(int value)
{
m_DebugMonitorEnabled = value;
}
bool IsDebugMonitor()
{
return IsServer() && m_DebugMonitorEnabled;
}
proto native void GetPlayerIndentities( out array<PlayerIdentity> identities );
proto native bool GetSurface(SurfaceDetectionParameters params, SurfaceDetectionResult result);
proto native float SurfaceY(float x, float z);
proto native float SurfaceRoadY(float x, float z, RoadSurfaceDetection rsd = RoadSurfaceDetection.LEGACY);
proto native float SurfaceRoadY3D(float x, float y, float z, RoadSurfaceDetection rsd);
proto float SurfaceGetType(float x, float z, out string type);
proto float SurfaceGetType3D(float x, float y, float z, out string type);
proto void SurfaceUnderObject(notnull Object object, out string type, out int liquidType);
proto void SurfaceUnderObjectEx(notnull Object object, out string type, out string impact, out int liquidType);
proto void SurfaceUnderObjectByBone(notnull Object object, int boneType, out string type, out int liquidType);
proto native float SurfaceGetNoiseMultiplier(Object directHit, vector pos, int componentIndex);
proto native vector SurfaceGetNormal(float x, float z);
proto native float SurfaceGetSeaLevelMin();
proto native float SurfaceGetSeaLevelMax();
proto native float SurfaceGetSeaLevel();
proto native float SurfaceGetSeaWaveMax();
proto native float SurfaceGetSeaWaveCurrent();
proto native bool SurfaceIsSea(float x, float z);
proto native bool SurfaceIsPond(float x, float z);
proto native float GetWaterDepth(vector posWS);
proto native float GetWaterSurfaceHeightNoFakeWave(vector posWS);
proto native float GetWaterSurfaceHeightWithFakeWave(vector posWS);
proto native void UpdatePathgraphRegion(vector regionMin, vector regionMax);
float GetHighestSurfaceYDifference( array<vector> positions)
{
float high = -9999999;
float low = 99999999;
for (int i = 0; i < positions.Count(); i++)
{
vector pos = positions.Get(i);
pos[1] = SurfaceRoadY( pos[0], pos[2]);
float y = pos[1];
if ( y > high )
high = y;
if ( y < low )
low = y;
;
}
return high - low;
}
vector GetSurfaceOrientation(float x, float z)
{
vector normal = g_Game.SurfaceGetNormal(x, z);
vector angles = normal.VectorToAngles();
angles[1] = angles[1]+270; // This fixes rotation of item so it stands vertically. Feel free to change, but fix existing use cases.
//Hack because setorientation and similar functions break and flip stuff upside down when using exactly this vector ¯\_(ツ)_/¯ (note: happens when surface is flat)
if (angles == "0 540 0")
angles = "0 0 0";
return angles;
}
bool IsSurfaceDigable(string surface)
{
return ConfigGetInt("CfgSurfaces " + surface + " isDigable");
}
bool GetSurfaceDigPile(string surface, out string result)
{
return ConfigGetText("CfgSurfaces " + surface + " digPile", result);
}
bool IsSurfaceFertile(string surface)
{
return ConfigGetInt("CfgSurfaces " + surface + " isFertile");
}
int CorrectLiquidType(int liquidType)
{
if (liquidType == -1)
return LIQUID_NONE;
if (liquidType == 0)
return LIQUID_SALTWATER;
return liquidType;
}
void SurfaceUnderObjectCorrectedLiquid(notnull Object object, out string type, out int liquidType)
{
SurfaceUnderObject(object, type, liquidType);
liquidType = CorrectLiquidType(liquidType);
}
void SurfaceUnderObjectExCorrectedLiquid(notnull Object object, out string type, out string impact, out int liquidType)
{
SurfaceUnderObjectEx(object, type, impact, liquidType);
liquidType = CorrectLiquidType(liquidType);
}
void SurfaceUnderObjectByBoneCorrectedLiquid(notnull Object object, int boneType, out string type, out int liquidType)
{
SurfaceUnderObjectByBone(object, boneType, type, liquidType);
liquidType = CorrectLiquidType(liquidType);
}
void UpdatePathgraphRegionByObject(Object object)
{
if ( object )
{
vector pos = object.GetPosition();
vector min_max[2];
float radius = object.ClippingInfo ( min_max );
vector min = Vector ( pos[0] - radius, pos[1], pos[2] - radius );
vector max = Vector ( pos[0] + radius, pos[1], pos[2] + radius );
UpdatePathgraphRegion( min, max );
}
}
proto native bool IsBoxColliding(vector center, vector orientation, vector edgeLength, array<Object> excludeObjects, array<Object> collidedObjects = NULL);
proto native bool IsBoxCollidingGeometry(vector center, vector orientation, vector edgeLength, int iPrimaryType, int iSecondaryType, array<Object> excludeObjects, array<Object> collidedObjects = NULL);
proto native bool IsBoxCollidingGeometryProxy(notnull BoxCollidingParams params, array<Object> excludeObjects, array<ref BoxCollidingResult> collidedObjects = NULL);
proto native Weather GetWeather();
proto native void SetEVUser(float value);
proto native void OverrideDOF(bool enable, float focusDistance, float focusLength, float focusLengthNear, float blur, float focusDepthOffset);
proto native void AddPPMask(float ndcX, float ndcY, float ndcRadius, float ndcBlur);
proto native void ResetPPMask();
proto native void OverrideInventoryLights(vector diffuse, vector ambient, vector ground, vector dir);
proto native void NightVissionLightParams(float lightIntensityMul, float noiseIntensity);
proto native void OpenURL(string url);
proto native void InitDamageEffects(Object effect);
//-----------------------------------------------------------------------------
// persitence
//-----------------------------------------------------------------------------
proto native EntityAI GetEntityByPersitentID( int b1, int b2, int b3, int b4 );
//-----------------------------------------------------------------------------
bool IsKindOf(string cfg_class_name, string cfg_parent_name)
{
TStringArray full_path = new TStringArray;
ConfigGetFullPath("CfgVehicles " + cfg_class_name, full_path);
if (full_path.Count() == 0)
{
ConfigGetFullPath("CfgAmmo " + cfg_class_name, full_path);
}
if (full_path.Count() == 0)
{
ConfigGetFullPath("CfgMagazines " + cfg_class_name, full_path);
}
if (full_path.Count() == 0)
{
ConfigGetFullPath("cfgWeapons " + cfg_class_name, full_path);
}
if (full_path.Count() == 0)
{
ConfigGetFullPath("CfgNonAIVehicles " + cfg_class_name, full_path);
}
cfg_parent_name.ToLower();
int nFullPath = full_path.Count();
for (int i = 0; i < nFullPath; i++)
{
string tmp = full_path.Get(i);
tmp.ToLower();
if (tmp == cfg_parent_name)
{
return true;
}
}
return false;
}
bool ObjectIsKindOf(Object object, string cfg_parent_name)
{
TStringArray full_path = new TStringArray;
ConfigGetObjectFullPath(object, full_path);
cfg_parent_name.ToLower();
int nFullPath = full_path.Count();
for (int i = 0; i < nFullPath; i++)
{
string tmp = full_path.Get(i);
tmp.ToLower();
if (tmp == cfg_parent_name)
{
return true;
}
}
return false;
}
int ConfigFindClassIndex(string config_path, string searched_member)
{
int class_count = ConfigGetChildrenCount(config_path);
for (int index = 0; index < class_count; index++)
{
string found_class = "";
ConfigGetChildName(config_path, index, found_class);
if (found_class == searched_member)
{
return index;
}
}
return -1;
}
proto int GetTime();
ScriptCallQueue GetCallQueue(int call_category) {}
ScriptInvoker GetUpdateQueue(int call_category) {}
ScriptInvoker GetPostUpdateQueue(int call_category) {}
TimerQueue GetTimerQueue(int call_category) {}
DragQueue GetDragQueue() {}
string CreateDefaultPlayer() {}
string CreateRandomPlayer() {}
TStringArray ListAvailableCharacters() {}
bool IsInventoryOpen()
{
}
proto native BiosUserManager GetUserManager();
proto native ContentDLC GetContentDLCService();
bool IsMissionMainMenu()
{
return (g_Game.GetMissionState() == DayZGame.MISSION_STATE_MAINMENU);
}
MenuDefaultCharacterData GetMenuDefaultCharacterData(bool fill_data = true)
{
//Print("GetMenuDefaultCharacterData");
//DumpStack();
//if used on server, creates an empty container to be filled by received data
if (!m_CharacterData)
{
m_CharacterData = new MenuDefaultCharacterData;
if (fill_data && !g_Game.IsDedicatedServer())
GetMenuData().RequestGetDefaultCharacterData(); //fills the structure
}
return m_CharacterData;
}
//Analytics Manager
AnalyticsManagerServer GetAnalyticsServer()
{
return m_AnalyticsManagerServer;
}
AnalyticsManagerClient GetAnalyticsClient()
{
return m_AnalyticsManagerClient;
}
/*
Returns the value set in server config for the length of the exit button timeout after cancelling the previous logout attempt (consoles only)
*/
proto native int GetLogoutAfterCancelTimeout();
};