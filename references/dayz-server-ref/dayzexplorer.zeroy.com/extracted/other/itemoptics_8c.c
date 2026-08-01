// ======================================================================
// File: itemoptics_8c_source.html
// Category: other
// ======================================================================

class ItemOptics : InventoryItemSuper
{
bool m_data_set;
bool m_allowsDOF; //true if optics DOES NOT have magnification (FOV >= DZPLAYER_CAMERA_FOV_IRONSIGHTS)
bool m_reddot_displayed
bool m_isNVOptic = false;
int m_CurrentOpticMode; //generic optic mode, currently used for NV optics only (could be expanded)
int m_CurrentOpticModeLocal; //local mirror for sync purposes;
int m_reddot_index;
float m_blur_float;
float m_nearplane_override; //override value for DayZPlayerCameraOptics only!
string m_optic_sight_texture;
string m_optic_sight_material;
string m_2D_preload_type;
ref array<float> m_mask_array;
ref array<float> m_lens_array;
ref array<float> m_OpticsDOFProperties;
void ItemOptics()
{
m_mask_array = new array<float>;
m_lens_array = new array<float>;
m_OpticsDOFProperties = new array<float>;
InitReddotData();
InitOpticsPPInfo();
InitCameraOverrideProperties();
InitOpticsDOFProperties(m_OpticsDOFProperties);
Init2DPreloadType();
InitOpticMode();
m_CurrentOpticModeLocal = -1;
RegisterNetSyncVariableInt( "m_CurrentOpticMode", 0, 63 );
}
proto native bool EnterOptics ();
proto native bool IsInOptics ();
proto native bool ExitOptics ();
proto native bool HasWeaponIronsightsOverride ();
proto native bool UseWeaponIronsightsOverride(bool state);
proto native bool IsUsingWeaponIronsightsOverride();
proto native int GetStepFOVCount ();
proto native int GetStepFOVIndex ();
proto native bool SetStepFOVIndex (int step);
proto native bool StepFOVUp ();
proto native bool StepFOVDown ();
proto native float GetCurrentStepFOV ();
proto native int GetStepZeroing ();
proto native bool SetStepZeroing (int step);
proto native bool StepZeroingUp ();
proto native bool StepZeroingDown ();
proto native void GetCameraPoint (out vector pos, out vector dir);
proto native float GetZoomInit();
proto native float GetZoomMin();
proto native float GetZoomMax();
proto native float GetZeroingDistanceZoomMin();
proto native float GetZeroingDistanceZoomMax();
proto native void SetZeroingClampDist(float dist);
protected void OnDrawOptics2D()
{
if (m_reddot_index == -1)
return;
ItemOpticsType opticsType = GetItemOpticsType();
if (m_reddot_displayed)
{
if (m_optic_sight_texture != "")
opticsType.SetOptics2DTexture(m_reddot_index, m_optic_sight_texture);
if (m_optic_sight_material != "")
opticsType.SetOptics2DMaterial(m_reddot_index, m_optic_sight_material);
}
else
{
opticsType.SetOptics2DTexture(m_reddot_index, "");
opticsType.SetOptics2DMaterial(m_reddot_index, "");
}
}
protected ItemOpticsType GetItemOpticsType()
{
return ItemOpticsType.Cast(GetInventoryItemType());
}
bool IsUsingOptics2DModel()
{
ItemOpticsType type = GetItemOpticsType();
return type.IsUsingOptics2DModel();
}
bool IsSightOnly()
{
return false;
}
bool IsUsableWithNV()
{
return false;
}
override void OnWorkStart()
{
if (!g_Game.IsDedicatedServer())
{
ShowReddot(true);
}
}
override void OnWorkStop()
{
if (!g_Game.IsDedicatedServer())
{
ShowReddot(false);
}
}
bool IsWorking()
{
if (GetCompEM() && GetCompEM().CanWork())
return true;
return false;
}
void UpdateOpticsReddotVisibility()
{
if (IsWorking() && !m_reddot_displayed)
{
ShowReddot(true);
}
else if (!IsWorking() && m_reddot_displayed)
{
ShowReddot(false);
}
}
override void OnWasAttached( EntityAI parent, int slot_id )
{
super.OnWasAttached(parent, slot_id);
SetTakeable(false);
Weapon wep;
if (Class.CastTo(wep,parent))
{
SetZeroingClampDist(wep.GetZeroingClamp(wep.GetCurrentMuzzle()));
}
}
override void OnWasDetached( EntityAI parent, int slot_id )
{
super.OnWasDetached(parent, slot_id);
PlayerBase player;
if (PlayerBase.CastTo(player, GetHierarchyRootPlayer()))
{
player.SetReturnToOptics(false);
}
SetTakeable(true);
Weapon wep;
if (Class.CastTo(wep,parent))
{
SetZeroingClampDist(0.0);
}
}
override void OnInventoryExit(Man player)
{
super.OnInventoryExit(player);
PlayerBase playerPB;
if (PlayerBase.CastTo(playerPB, player))
{
playerPB.SetReturnToOptics(false);
}
SetTakeable(true);
}
override void OnStoreSave(ParamsWriteContext ctx)
{
super.OnStoreSave(ctx);
ctx.Write(m_CurrentOpticMode);
}
override bool OnStoreLoad(ParamsReadContext ctx, int version)
{
if ( !super.OnStoreLoad(ctx,version) )
{
return false;
}
m_IsStoreLoad = true;
if ( version >= 126 )
{
if ( !ctx.Read(m_CurrentOpticMode) )
{
m_IsStoreLoad = false;
return false;
}
}
OnOpticModeChange();
SetSynchDirty();
m_IsStoreLoad = false;
return true;
}
override void OnVariablesSynchronized()
{
super.OnVariablesSynchronized();
if (m_CurrentOpticModeLocal != m_CurrentOpticMode)
{
OnOpticModeChange();
m_CurrentOpticModeLocal = m_CurrentOpticMode;
}
}
void InitReddotData()
{
bool isUsing2D = IsUsingOptics2DModel();
string path;
if (isUsing2D)
{
path = "cfgVehicles " + GetType() + " OpticsModelInfo";
}
else
{
path = "cfgVehicles " + GetType() + " OpticsInfo";
}
string temp;
if (g_Game.ConfigIsExisting(path))
{
if (isUsing2D)
m_reddot_index = GetItemOpticsType().FindOptics2DSelection("reddot");
else
m_reddot_index = GetHiddenSelectionIndex("reddot");
if (g_Game.ConfigIsExisting(path + " opticSightTexture"))
{
g_Game.ConfigGetText(path + " opticSightTexture", temp);
m_optic_sight_texture = temp;
temp = "";
}
if (g_Game.ConfigIsExisting(path + " opticSightMaterial"))
{
g_Game.ConfigGetText(path + " opticSightMaterial", temp);
m_optic_sight_material = temp;
temp = "";
}
}
m_data_set = true;
}
void ShowReddot(bool state)
{
if (g_Game.IsDedicatedServer())
{
ErrorEx("should not be called on the server!",ErrorExSeverity.INFO);
return;
}
if (!m_data_set)
{
InitReddotData();
}
// does not have reddot
if (m_reddot_index == -1)
{
return;
}
// 2D model has special handling in `OnDrawOptics2D`
if (!IsUsingOptics2DModel())
{
if (state)
{
if (m_optic_sight_texture != "")
SetObjectTexture(m_reddot_index, m_optic_sight_texture);
if (m_optic_sight_material != "")
SetObjectMaterial(m_reddot_index, m_optic_sight_material);
}
else
{
SetObjectTexture(m_reddot_index, "");
SetObjectMaterial(m_reddot_index, "");
}
}
m_reddot_displayed = state;
}
void InitOpticsPPInfo()
{
m_allowsDOF = InitDOFAvailability();
InitOpticsPP(m_mask_array, m_lens_array, m_blur_float);
m_isNVOptic = ConfigGetBool("NVOptic");
}
bool InitDOFAvailability()
{
float fov_max;
string path = "cfgVehicles " + GetType() + " OpticsInfo";
/*
Weapon_Base weapon = Weapon_Base.Cast(GetHierarchyParent());
if (!weapon)
return false; // no DOF for handheld optics
*/
fov_max = g_Game.ConfigGetFloat(path + " opticsZoomMax");
if (fov_max >= GameConstants.DZPLAYER_CAMERA_FOV_IRONSIGHTS)
{
return true;
}
return false;
}
void InitOpticsPP(out array<float> mask_array, out array<float> lens_array, out float blur_float)
{
string path = "cfgVehicles " + GetType() + " OpticsInfo";
g_Game.ConfigGetFloatArray(path + " PPMaskProperties", mask_array);
g_Game.ConfigGetFloatArray(path + " PPLensProperties", lens_array);
blur_float = g_Game.ConfigGetFloat(path + " PPBlurProperties");
}
void InitCameraOverrideProperties()
{
string path = "cfgVehicles " + GetType() + " OpticsInfo";
if ( g_Game.ConfigIsExisting(path + " nearPlaneDistanceOverride") )
{
m_nearplane_override = Math.Max(g_Game.ConfigGetFloat(path + " nearPlaneDistanceOverride"),DayZPlayerCameraBase.CONST_NEARPLANE_OPTICS_MIN);
}
else
{
m_nearplane_override = DayZPlayerCameraOptics.CONST_NEARPLANE_OPTICS;
}
}
bool InitOpticsDOFProperties (out array<float> temp_array)
{
if (g_Game.ConfigIsExisting("cfgVehicles " + GetType() + " OpticsInfo PPDOFProperties"))
{
g_Game.ConfigGetFloatArray("cfgVehicles " + GetType() + " OpticsInfo PPDOFProperties", temp_array);
return true;
}
else if (g_Game.ConfigIsExisting("cfgVehicles " + GetType() + " OpticsInfoWeaponOverride PPDOFProperties"))
{
g_Game.ConfigGetFloatArray("cfgVehicles " + GetType() + " OpticsInfoWeaponOverride PPDOFProperties", temp_array);
return true;
}
return false;
}
bool AllowsDOF()
{
return m_allowsDOF;
}
bool IsNVOptic()
{
return m_isNVOptic;
}
int GetCurrentNVType()
{
/*
//TODO - implement this into NV optics and modify (KazuarOptic example below)
if (IsWorking())
{
switch (m_CurrentOpticMode)
{
case GameConstants.OPTICS_STATE_DAY:
return NVTypes.NV_OPTICS_KAZUAR_DAY;
case GameConstants.OPTICS_STATE_NIGHTVISION:
return NVTypes.NV_OPTICS_KAZUAR_NIGHT;
}
}
else
{
return NVTypes.NV_OPTICS_OFF;
}
*/
return NVTypes.NONE;
}
void SetCurrentOpticMode(int mode)
{
m_CurrentOpticMode = mode;
OnOpticModeChange();
}
int GetCurrentOpticMode()
{
return m_CurrentOpticMode;
}
void OnOpticModeChange(){}
void OnOpticEnter()
{
if ( g_Game && !g_Game.IsDedicatedServer() )
{
HideSelection("hide");
}
}
void OnOpticExit()
{
if ( g_Game && !g_Game.IsDedicatedServer() )
{
ShowSelection("hide");
}
}
ref array<float> GetOpticsDOF()
{
return m_OpticsDOFProperties;
}
ref array<float> GetOpticsPPMask()
{
return m_mask_array;
}
ref array<float> GetOpticsPPLens()
{
return m_lens_array;
}
float GetOpticsPPBlur()
{
return m_blur_float;
}
float GetNearPlaneValue()
{
return m_nearplane_override;
}
void Init2DPreloadType()
{
string path = "cfgVehicles " + GetType() + " OpticsInfo preloadOpticType";
string type_2d;
if ( g_Game.ConfigIsExisting(path) )
{
g_Game.ConfigGetText(path, type_2d);
m_2D_preload_type = type_2d;
}
}
void InitOpticMode()
{
SetCurrentOpticMode(GameConstants.OPTICS_STATE_DAY);
}
void UpdateSelectionVisibility() {}
override void SetActions()
{
super.SetActions();
AddAction(ActionViewOptics);
}
override void OnDebugSpawn()
{
GetInventory().CreateAttachment("Battery9V");
}
}
typedef ItemOptics OpticBase;