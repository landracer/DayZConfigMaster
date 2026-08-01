// ======================================================================
// File: object_8c_source.html
// Category: other
// ======================================================================

enum ProcessDirectDamageFlags
{
ALL_TRANSFER,
NO_ATTACHMENT_TRANSFER,
NO_GLOBAL_TRANSFER,
NO_TRANSFER,
}
class ObjectSnapCallback
{
Object m_Owner;
vector m_OwnerPosition;
vector m_OwnerDirection;
vector m_Offset;
vector m_Extents;
vector m_DirectionFunc;
bool m_DebugEnabled;
vector m_Transform[4];
void OnSetup()
{
}
void OnDebug(vector p0, vector p1, bool hasHit, bool found)
{
}
bool OnFirstContact(Object other)
{
return false;
}
bool OnQuery(Object other)
{
return true;
}
bool OnCollide(Object other)
{
return true;
}
};
class Object extends IEntity
{
private void ~Object();
private void Object();
bool CanBeSkinned()
{
return false;
}
void Delete()
{
g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(g_Game.ObjectDelete, this);
}
proto native void AddProxyPhysics(string proxySelectionName);
proto native void RemoveProxyPhysics(string proxySelectionName);
void OnEnterTrigger(ScriptedEntity trigger) {}
void OnLeaveTrigger(ScriptedEntity trigger) {}
proto native bool GetLODS(notnull out array<LOD> lods);
proto native owned string GetLODName(LOD lod);
proto native vector GetBoundingCenter();
LOD GetLODByName( string name )
{
array<LOD> lods = new array<LOD>;
GetLODS( lods );
for ( int i = 0; i < lods.Count(); ++i )
{
string lod_name = GetLODName( lods.Get( i ) );
lod_name.ToLower();
name.ToLower();
if ( lod_name == name )
{
return lods.Get( i );
}
}
return NULL;
}
TStringArray GetHiddenSelections()
{
return HiddenSelectionsData.GetHiddenSelectionsConfig(GetType());
}
TStringArray GetHiddenSelectionsTextures()
{
return HiddenSelectionsData.GetHiddenSelectionsTexturesConfig(GetType());
}
TStringArray GetHiddenSelectionsMaterials()
{
return HiddenSelectionsData.GetHiddenSelectionsMaterialsConfig(GetType());
}
void Explode(int damageType, string ammoType = "")
{
if (ammoType == "")
ammoType = ConfigGetString("ammoType");
if (ammoType == "")
ammoType = "Dummy_Heavy";
if ( g_Game.IsServer() )
{
SynchExplosion();
DamageSystem.ExplosionDamage(EntityAI.Cast(this), null, ammoType, GetPosition(), damageType);
}
}
void SynchExplosion()
{
if ( g_Game.IsDedicatedServer() ) // Multiplayer server
{
Param1<EntityAI> p = new Param1<EntityAI>(null);
g_Game.RPCSingleParam( this, ERPCs.RPC_EXPLODE_EVENT, p, true);
}
else if ( !g_Game.IsMultiplayer() ) // Singleplayer
{
OnExplodeClient();
}
}
void OnExplodeClient()
{
string ammoType = ConfigGetString("ammoType");
if (ammoType == "")
ammoType = "Dummy_Heavy";
vector pos = GetPosition();
// Handle spawn of particle if one is configured in config
AmmoEffects.PlayAmmoParticle(ammoType, pos);
// Handle spawn of Effect if one is configured in config
AmmoEffects.PlayAmmoEffect(ammoType, pos);
}
void OnExplosionEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, float energyFactor, float explosionFactor, bool isWater, string ammoType) { }
proto native owned string GetActionComponentName(int componentIndex, string geometry = "");
proto native owned vector GetActionComponentPosition(int componentIndex, string geometry = "");
proto native owned int GetActionComponentNameList(int componentIndex, TStringArray nameList, string geometry = "");
proto native bool IsActionComponentPartOfSelection(int componentIndex, string selectionName, string geometry = "");
proto void GetActionComponentsForSelectionName(int level, string selectionName, TIntArray componentIndices);
proto vector GetActionComponentCenter(int level, int componentIndex);
proto vector GetActionComponentCenterOOB(int level, int componentIndex);
proto void GetActionComponentMinMax(int level, int componentIndex, out vector min, out vector max);
proto native bool ToDelete();
proto native bool IsPendingDeletion();
proto native int GetGeometryLevel();
proto native int GetFireGeometryLevel();
proto native int GetViewGeometryLevel();
proto native int GetMemoryLevel();
#ifdef DEVELOPER
proto bool ToBonePivot(out int pivot, int level, int bone);
proto bool FromBonePivot(int pivot, out int level, out int bone);
#endif
proto int GetBonePivot(int level, int component);
proto native void GetBonePivotsForAnimationSource(int level, string animationSource, out TIntArray pivots);
proto native vector GetBonePositionLS(int pivot);
proto native vector GetBonePositionMS(int pivot);
proto native vector GetBonePositionWS(int pivot);
proto native void GetBoneRotationLS(int pivot, out float quaternion[4]);
proto native void GetBoneRotationMS(int pivot, out float quaternion[4]);
proto native void GetBoneRotationWS(int pivot, out float quaternion[4]);
proto native void GetBoneTransformLS(int pivot, out vector transform[4]);
proto native void GetBoneTransformMS(int pivot, out vector transform[4]);
proto native void GetBoneTransformWS(int pivot, out vector transform[4]);
proto native void GetTightlyPackedCorners(ETransformationAxis axis, out vector corners[4]);
#ifdef DIAG_DEVELOPER
void DebugDrawTightlyPackedCorners(ETransformationAxis axis, int color)
{
#ifndef SERVER
vector points[2];
vector corners[4];
GetTightlyPackedCorners(axis, corners);
points[0] = corners[0];
points[1] = corners[1];
Shape.CreateLines(color, ShapeFlags.TRANSP | ShapeFlags.ONCE | ShapeFlags.NOOUTLINE | ShapeFlags.NOZBUFFER, points, 2);
points[0] = corners[1];
points[1] = corners[3];
Shape.CreateLines(color, ShapeFlags.TRANSP | ShapeFlags.ONCE | ShapeFlags.NOOUTLINE | ShapeFlags.NOZBUFFER, points, 2);
points[0] = corners[3];
points[1] = corners[2];
Shape.CreateLines(color, ShapeFlags.TRANSP | ShapeFlags.ONCE | ShapeFlags.NOOUTLINE | ShapeFlags.NOZBUFFER, points, 2);
points[0] = corners[2];
points[1] = corners[0];
Shape.CreateLines(color, ShapeFlags.TRANSP | ShapeFlags.ONCE | ShapeFlags.NOOUTLINE | ShapeFlags.NOZBUFFER, points, 2);
#endif
}
#endif
proto native vector GetPosition();
proto native vector GetWorldPosition();
proto native void SetPosition(vector vPos);
proto native void PlaceOnSurface();
proto native vector GetOrientation();
proto native void SetOrientation(vector vOrientation);
proto native vector GetDirection();
proto native void SetDirection(vector vPos);
proto native vector GetDirectionUp();
proto native vector GetDirectionAside();
proto native vector GetLocalPos(vector vPos);
proto native vector GetGlobalPos(vector vPos);
proto native vector GetSpeed();
proto native vector GetModelSpeed();
proto native bool CanAffectPathgraph();
proto native void SetAffectPathgraph(bool fromConfig, bool affect);
proto float ClippingInfo(out vector minMax[2]);
proto native bool GetCollisionBox(out vector minMax[2]);
proto native float GetCollisionRadius();
proto native float GetDamage();
proto native void GetSelectionList(out TStringArray selectionList);
float GetSurfaceNoise()
{
vector position = GetPosition();
return g_Game.SurfaceGetNoiseMultiplier(NULL, position, -1);
}
string GetSurfaceType()
{
string surface_type;
int liquid_type;
g_Game.SurfaceUnderObject(this, surface_type,liquid_type);
// Print(surface_type);
// Print(liquid_type);
return surface_type;
}
bool HasSelection( string selection )
{
TStringArray selections = new TStringArray;
GetSelectionList( selections );
for ( int i = 0; i < selections.Count(); ++i )
{
if ( selections.Get( i ) == selection )
{
return true;
}
}
return false;
}
bool HasAnimation( string anim_name )
{
string cfg_path_vehicles = "CfgVehicles " + GetType() + " ";
if ( g_Game.ConfigIsExisting (cfg_path_vehicles) && g_Game.ConfigIsExisting (cfg_path_vehicles + "AnimationSources " + anim_name) )
{
return true;
}
string cfg_path_weapons = "CfgWeapons " + GetType() + " ";
if ( g_Game.ConfigIsExisting (cfg_path_weapons) && g_Game.ConfigIsExisting (cfg_path_weapons + "AnimationSources " + anim_name) )
{
return true;
}
string cfg_path_magazines = "CfgMagazines " + GetType() + " ";
if ( g_Game.ConfigIsExisting (cfg_path_magazines) && g_Game.ConfigIsExisting (cfg_path_magazines + "AnimationSources " + anim_name) )
{
return true;
}
return false;
}
/*bool HasEnergyManager()
{
return false;
}*/
proto native int GetMemoryPointIndex(string memoryPointName);
proto native vector GetMemoryPointPos(string memoryPointName);
proto native vector GetMemoryPointPosByIndex(int pointIndex);
proto native bool MemoryPointExists(string memoryPoint);
proto native void CreateDynamicPhysics(int interactionLayers);
proto native void EnableDynamicCCD(bool state);
proto native void SetDynamicPhysicsLifeTime(float lifeTime);
void OnTreeCutDown(EntityAI cutting_entity);
proto native EntityType GetEntityType();
string GetType()
{
string ret;
g_Game.ObjectGetType(this, ret);
return ret;
}
string GetDisplayName()
{
string tmp;
if (NameOverride(tmp))
{
tmp = Widget.TranslateString(tmp);
}
else
{
g_Game.ObjectGetDisplayName(this, tmp);
}
return tmp;
}
//Used to specify this object will always display it&#39;s own name when attached to another object
bool DisplayNameRuinAttach()
{
return false;
}
string GetModelName()
{
return g_Game.GetModelName(GetType());
}
proto native owned string GetShapeName();
int Release()
{
return g_Game.ObjectRelease(this);
}
bool IsKindOf(string type)
{
return g_Game.ObjectIsKindOf(this, type);
}
// Check alive state
bool IsAlive()
{
return !IsDamageDestroyed();
}
bool IsMan()
{
return false;
}
bool IsDayZCreature()
{
return false;
}
bool IsEntityAI()
{
return false;
}
bool IsStaticTransmitter()
{
return false;
}
bool IsTransmitter()
{
return false;
}
bool IsItemBase()
{
return false;
}
//Returns true for protector cases and similar items. Usually can be nested in other cargo while full, unlike clothing..
bool IsContainer()
{
return false;
}
bool IsMagazine()
{
return false;
}
bool IsAmmoPile()
{
return false;
}
bool IsInventoryItem()
{
return false;
}
bool IsClothing()
{
return false;
}
bool IsFireplace()
{
return false;
}
bool IsTree()
{
return false;
}
bool IsRock()
{
return false;
}
bool IsWoodBase()
{
return false;
}
bool IsBush()
{
return false;
}
bool IsCuttable()
{
return false;
}
bool IsPeltBase()
{
return false;
}
bool IsWeapon()
{
return false;
}
bool IsMeleeWeapon()
{
return false;
}
bool IsBuilding()
{
return false;
}
EWaterSourceObjectType GetWaterSourceObjectType()
{
return EWaterSourceObjectType.NONE;
}
int GetLiquidSourceType()
{
return LIQUID_NONE;
}
bool IsWell()
{
return false;
}
bool ShootsExplosiveAmmo()//placed on Object so that we can optimize early checks in DayZGame without casting
{
return false;
}
bool IsFuelStation()
{
return false;
}
bool IsTransport()
{
return false;
}
bool IsElectricAppliance()
{
return false;
}
bool CanUseConstruction()
{
return false;
}
bool CanUseConstructionBuild()
{
return false;
}
bool CanUseHandConstruction()
{
return false;
}
bool IsBeingBackstabbed()
{
return false;
}
void SetBeingBackstabbed(int backstabType){}
bool IsFood()
{
return ( IsFruit() || IsMeat() || IsCorpse() || IsMushroom() );
}
bool IsFruit()
{
return false;
}
bool IsMeat()
{
return false;
}
bool IsCorpse()
{
return false;
}
bool IsMushroom()
{
return false;
}
bool IsHealthVisible()
{
return true;
}
bool ShowZonesHealth()
{
return false;
}
bool IsParticle()
{
return false;
}
bool IsItemTent()
{
return false;
}
bool IsScriptedLight()
{
return false;
}
bool IsHologram()
{
return false;
}
bool HasProxyParts()
{
return false;
}
bool CanObstruct()
{
return IsPlainObject() && !IsScenery();
}
bool CanProxyObstruct()
{
return HasProxyParts() || CanUseConstruction();
}
bool CanProxyObstructSelf()
{
return false;
}
bool CanBeIgnoredByDroppedItem()
{
return IsBush() || IsTree() || IsMan() || IsDayZCreature() || IsItemBase();
}
bool CanBeAutoDeleted()
{
return true;
}
bool DisableVicinityIcon()
{
return false;
}
proto void GetNetworkID( out int lowBits, out int highBits );
string GetNetworkIDString()
{
int low, high;
GetNetworkID( low, high );
return high.ToString() + low.ToString();
}
override string GetDebugName()
{
return GetDebugNameNative();
}
static string GetDebugName(Object o)
{
if (o)
return o.GetDebugName();
return "null";
}
proto string GetDebugNameNative();
void RPC(int rpc_type, array<ref Param> params, bool guaranteed, PlayerIdentity recipient = NULL)
{
g_Game.RPC(this, rpc_type, params, guaranteed, recipient);
}
void RPCSingleParam(int rpc_type, Param param, bool guaranteed, PlayerIdentity recipient = NULL)
{
g_Game.RPCSingleParam(this, rpc_type, param, guaranteed, recipient);
}
void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx);
proto vector GetSelectionPositionOld(string name);
proto vector GetSelectionPositionLS(string name);
proto vector GetSelectionPositionMS(string name);
proto vector GetSelectionPositionWS(string name);
proto vector GetSelectionBasePositionLS(string name);
proto vector ModelToWorld(vector modelPos);
proto vector WorldToModel(vector worldPos);
// config class API
proto string ConfigGetString(string entryName);
proto string ConfigGetStringRaw(string entryName);
proto int ConfigGetInt(string entryName);
bool ConfigGetBool(string entryName)
{
return (ConfigGetInt(entryName) == 1);
}
proto float ConfigGetFloat(string entryName);
proto vector ConfigGetVector(string entryName);
//proto ParamEntry ConfigGetEntry(string entryName);
proto native void ConfigGetTextArray(string entryName, out TStringArray values);
proto native void ConfigGetTextArrayRaw(string entryName, out TStringArray values);
proto native void ConfigGetFloatArray(string entryName, out TFloatArray values);
proto native void ConfigGetIntArray(string entryName, out TIntArray values);
proto native bool ConfigIsExisting(string entryName);
bool KindOf( string tag )
{
string item_name = this.GetType();
TStringArray item_tag_array = new TStringArray;
ConfigGetTextArray("cfgVehicles " + item_name + " itemInfo", item_tag_array);
int array_size = item_tag_array.Count();
for (int i = 0; i < array_size; i++)
{
if ( item_tag_array.Get(i) == tag )
{
return true;
}
}
return false;
}
bool IsAnyInherited( array<typename> typenames )
{
int nTypenames = typenames.Count();
for( int i = 0; i < nTypenames; ++i )
{
if (IsInherited(typenames.Get(i)))
{
return true;
}
}
return false;
}
proto native bool IsPlainObject();
proto native bool IsScenery();
// Damage system
proto native bool HasDamageSystem();
proto native bool IsDamageDestroyed();
proto native void SetDestructTime(float time);
proto native float GetHealth(string zoneName, string healthType);
proto native float GetHealth01(string zoneName, string healthType);
proto native float GetMaxHealth(string zoneName, string healthType);
proto native void SetHealth(string zoneName, string healthType, float value);
proto native void SetFullHealth();
proto native void AddHealth(string zoneName, string healthType, float value);
proto native void DecreaseHealth(string zoneName, string healthType, float value);
proto native bool GetAdditionalHealthTypes(string zoneName, TStringArray outHealthTypes);
void DecreaseHealth(string zoneName, string healthType, float value, bool auto_delete)
{
DecreaseHealth(zoneName, healthType, value);
if (auto_delete)
{
float result_health = GetHealth(zoneName, healthType);
if (result_health <= 0)
g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(g_Game.ObjectDelete, this);
}
}
float GetHealth()
{
return GetHealth("", "");
}
float GetHealth01()
{
return GetHealth01("", "");
}
float GetMaxHealth()
{
return GetMaxHealth("", "");
}
void SetHealth(float health)
{
SetHealth("", "", health);
}
void SetGlobalHealth(float health)
{
SetHealth("", "", health);
}
void SetHealthLevel(int healthLevel, string zone = "")
{
SetHealth01(zone,"", GetHealthLevelValue(healthLevel, zone));
}
void AddHealthLevel(int healthLevelDelta, string zone = "")
{
int maxHealthLevel = GetNumberOfHealthLevels(zone) - 1;
int newHealthLevel = Math.Clamp(GetHealthLevel(zone) + healthLevelDelta,0,maxHealthLevel);
SetHealthLevel(newHealthLevel,zone);
}
void SetHealth01(string zoneName, string healthType, float coef)
{
SetHealth(zoneName,healthType,(GetMaxHealth(zoneName,healthType)*coef));
}
void SetHealthMax(string zoneName = "", string healthType = "")
{
SetHealth(zoneName,healthType,GetMaxHealth(zoneName,healthType));
}
void AddHealth( float add_health)
{
AddHealth("", "", add_health);
}
void DecreaseHealth(float dec_health, bool auto_delete = true)
{
DecreaseHealth("", "", dec_health, auto_delete);
}
void DecreaseHealthCoef(float dec_health_coef, bool auto_delete = true)
{
float current_health = GetHealth();
float final_health_coef = GetHealth01() - dec_health_coef;
float final_health = GetMaxHealth() * final_health_coef;
DecreaseHealth("", "", current_health - final_health);
}
proto native void ProcessDirectDamage(int damageType, EntityAI source, string componentName, string ammoName, vector modelPos, float damageCoef = 1.0, int flags = 0);
bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
{
return true;
}
proto native void GetDamageZones(out TStringArray dmgZones);
proto native vector GetDamageZonePos(string zoneName);
proto native owned string GetDamageZoneNameByComponentIndex(int componentIndex);
proto native int GetHealthLevel(string zone = "");
proto native int GetNumberOfHealthLevels(string zone = "");
proto native float GetHealthLevelValue(int healthLevel, string zone = "");
proto native bool GetAllowDamage();
proto native void SetAllowDamage(bool val);
proto native bool GetCanBeDestroyed();
proto native void SetCanBeDestroyed(bool val);
bool IsRuined()
{
return IsDamageDestroyed();
}
void OnSimulationEnabled() {}
void OnSimulationDisabled() {}
void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
{
}
SoundOnVehicle PlaySound(string sound_name, float range, bool create_local = true)
{
return g_Game.CreateSoundOnObject(this, sound_name, range, false, create_local);
}
SoundOnVehicle PlaySoundLoop(string sound_name, float range, bool create_local = true)
{
return g_Game.CreateSoundOnObject(this, sound_name, range, true, create_local);
}
bool PlaySoundSet( out EffectSound sound, string sound_set, float fade_in, float fade_out, bool loop = false )
{
if ( g_Game && !g_Game.IsDedicatedServer() )
{
if ( sound )
{
if ( loop )
{
return true;
}
else
{
StopSoundSet( sound ); //auto stop for non-looped sounds
}
}
sound = SEffectManager.PlaySoundOnObject( sound_set, this, fade_in, fade_out, loop );
sound.SetAutodestroy( true );
return true;
}
return false;
}
bool PlaySoundSetLoop( out EffectSound sound, string sound_set, float fade_in, float fade_out )
{
return PlaySoundSet( sound, sound_set, fade_in, fade_out, true );
}
bool PlaySoundSetAtMemoryPointLoopedSafe(out EffectSound sound, string soundSet, string memoryPoint,float play_fade_in = 0, float stop_fade_out = 0)
{
if (sound && sound.IsPlaying())
{
sound.SoundStop();
}
return PlaySoundSetAtMemoryPointLooped(sound, soundSet, memoryPoint, play_fade_in, stop_fade_out);
}
bool PlaySoundSetAtMemoryPointLooped(out EffectSound sound, string soundSet, string memoryPoint, float play_fade_in = 0, float stop_fade_out = 0)
{
return PlaySoundSetAtMemoryPoint(sound, soundSet, memoryPoint, true, play_fade_in, stop_fade_out);
}
bool PlaySoundSetAtMemoryPoint(out EffectSound sound, string soundSet, string memoryPoint, bool looped = false, float play_fade_in = 0, float stop_fade_out = 0)
{
vector pos;
if (MemoryPointExists(memoryPoint))
{
pos = GetMemoryPointPos(memoryPoint);
pos = ModelToWorld(pos);
}
else
{
ErrorEx(string.Format("Memory point %1 not found when playing soundset %2 at memory point location", memoryPoint, soundSet));
return false;
}
sound = SEffectManager.PlaySoundEnviroment(soundSet, pos, play_fade_in, stop_fade_out, looped);
return true;
}
bool StopSoundSet( out EffectSound sound )
{
if ( sound && g_Game && ( !g_Game.IsDedicatedServer() ) )
{
sound.SoundStop();
sound = null;
return true;
}
return false;
}
void PostAreaDamageActions();
void PreAreaDamageActions();
void SpawnDamageDealtEffect();
void OnReceivedHit(ImpactEffectsData hitData);
void OnPlayerRecievedHit();
bool HasNetworkID()
{
int lo = 0;
int hi = 0;
GetNetworkID(lo, hi);
return lo | hi;
}
bool NameOverride(out string output)
{
return false;
}
bool DescriptionOverride(out string output)
{
return false;
}
EntityAI ProcessMeleeItemDamage(int mode = 0) {}
bool CanBeRepairedToPristine()
{
return false;
}
vector GetCenter()
{
if ( MemoryPointExists("ce_center") )
{
//Print("CE_CENTER found");
return ModelToWorld( GetMemoryPointPos("ce_center") );
}
else
{
//Print("CE_CENTER DOING A BAMBOOZLE => not found");
return GetPosition() + Vector(0, 0.2, 0);
}
}
#ifdef DEVELOPER
void SetDebugItem();
#endif
void AddArrow(Object arrow, int componentIndex, vector closeBonePosWS, vector closeBoneRotWS)
{
int pivot = GetBonePivot(GetFireGeometryLevel(), componentIndex);
vector parentTransMat[4];
vector arrowTransMat[4];
if (pivot == -1)
{
GetTransform(parentTransMat);
}
else
{
GetBoneTransformWS(pivot, parentTransMat);
}
float scale = GetScale();
scale = 1 / (scale * scale);
arrow.GetTransform(arrowTransMat);
Math3D.MatrixInvMultiply4(parentTransMat, arrowTransMat, arrowTransMat);
// orthogonalize matrix - parent might be skewed
Math3D.MatrixOrthogonalize4(arrowTransMat);
arrowTransMat[3] = arrowTransMat[3] * scale;
arrow.SetTransform(arrowTransMat);
AddChild(arrow, pivot);
}
bool CanBeActionTarget()
{
return !IsHologram();
}
bool HasFixedActionTargetCursorPosition()
{
return false;
}
void OnSpawnByObjectSpawner(ITEM_SpawnerObject item);
bool Gizmo_IsSupported()
{
return !g_Game.IsMultiplayer();
}
void Gizmo_SetWorldTransform(vector transform[4], bool finalize)
{
SetPosition(transform[3]);
SetDirection(transform[2]);
SetTransform(transform);
// TODO: RPC or something when finalize == true to send the new transform to the server, don&#39;t forget to make &#39;Gizmo_IsSupported&#39; return true
}
void Gizmo_GetWorldTransform(vector transform[4])
{
GetTransform(transform);
}
bool Gizmo_IsAllowedTransformMode(GizmoTransformMode mode)
{
return true;
}
bool Gizmo_IsAllowedSpaceMode(GizmoSpaceMode mode)
{
return true;
}
//Debug
//----------------------------------------------
/*void DbgAddPxyPhy(string slot)
{
Print("AddProxyPhysics slot: " + slot);
AddProxyPhysics(slot);
}*/
};