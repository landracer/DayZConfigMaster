// ======================================================================
// File: centraleconomy_8c_source.html
// Category: other
// ======================================================================

// -------------------------------------------------------------------------
// object (SF) Spawn Flags (use to setup behavior and/ or trigger functionality)
//
const int ECE_NONE = 0;
const int ECE_SETUP = 2; // process full entity setup (when creating NEW entity)
const int ECE_TRACE = 4; // trace under entity when being placed (from the point of creation)
const int ECE_CENTER = 8; // use center from shape (model) for placement
const int ECE_UPDATEPATHGRAPH = 32; // update navmesh when object placed upon it
const int ECE_ROTATIONFLAGS = 512; // enable rotation flags for object placement
const int ECE_CREATEPHYSICS = 1024; // create collision envelope and related physics data (if object has them)
const int ECE_INITAI = 2048; // init ai
const int ECE_AIRBORNE = 4096; // create flying unit in the air
const int ECE_EQUIP_ATTACHMENTS = 8192; // equip with configured ATTACHMENTS
const int ECE_EQUIP_CARGO = 16384; // equip with configured CARGO
const int ECE_EQUIP = 24576; // equip with configured (ATTACHMENTS + CARGO)
const int ECE_EQUIP_CONTAINER = 2097152; // populate DE/ group CONTAINER during spawn
const int ECE_LOCAL = 1073741824; // create object locally
const int ECE_NOSURFACEALIGN = 262144; // do not align object on surface/ terrain
const int ECE_KEEPHEIGHT = 524288; // keep height when creating object (do not use trace or placement on surface)
const int ECE_NOLIFETIME = 4194304; // do not set lifetime when creating the object
const int ECE_NOPERSISTENCY_WORLD = 8388608; // do not save this object in world
const int ECE_NOPERSISTENCY_CHAR = 16777216; // do not save this object in character
const int ECE_DYNAMIC_PERSISTENCY = 33554432; // spawns in without persistency, once player takes it, persistency will be enabled if available
// note: use predefined combination when not solving something specific
//
const int ECE_IN_INVENTORY = 787456; // ECE_CREATEPHYSICS|ECE_KEEPHEIGHT|ECE_NOSURFACEALIGN
const int ECE_PLACE_ON_SURFACE = 1060; // ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_TRACE
const int ECE_OBJECT_SWAP = 787488; // ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_KEEPHEIGHT|ECE_NOSURFACEALIGN
const int ECE_FULL = 25126; // ECE_SETUP|ECE_TRACE|ECE_ROTATIONFLAGS|ECE_UPDATEPATHGRAPH|ECE_EQUIP
// -------------------------------------------------------------------------
// object (RF) Rotation Flags (use to force and/ or invoke placement rotation)
//
const int RF_NONE = 0;
const int RF_FRONT = 1; // front side placement
const int RF_TOP = 2; // top side placement
const int RF_LEFT = 4; // left side placement
const int RF_RIGHT = 8; // right side placement
const int RF_BACK = 16; // back side placement
const int RF_BOTTOM = 32; // bottom side placement
const int RF_ALL = 63; // RF_FRONT|RF_TOP|RF_LEFT|RF_RIGHT|RF_BACK|RF_BOTTOM
const int RF_IGNORE = 64; // ignore placement RF flags - object will spawn as model was created
const int RF_TOPBOTTOM = 34; // RF_TOP|RF_BOTTOM
const int RF_LEFTRIGHT = 12; // RF_LEFT|RF_RIGHT
const int RF_FRONTBACK = 17; // RF_FRONT|RF_BACK
const int RF_RANDOMROT = 64; // allow random rotation around axis when placing
const int RF_ORIGINAL = 128; // use default placement setuped on object in config
const int RF_DECORRECTION = 256; // angle correction when spawning InventoryItem at Building angle
const int RF_DEFAULT = 512; // use default placement setuped on object in config
// -------------------------------------------------------------------------
class EconomyLogCategories
{
private void EconomyLogCategories();
private void ~EconomyLogCategories();
const string Economy = "economy";
const string EconomyRespawn = "economy_respawn";
const string RespawnQueue = "respawn_queue";
const string Container = "container";
const string Matrix = "matrix";
const string UniqueLoot = "uniqueloot";
const string Bind = "bind";
const string SetupFail = "setupfail";
const string Storage = "storage";
const string Classes = "class";
const string Category = "category";
const string Tag = "tag";
const string SCategory = "s_category";
const string STag = "s_tag";
const string SAreaflags = "s_areaflags";
const string SCrafted = "s_crafted";
const string MapGroup = "map_group";
const string MapComplete = "map_complete";
const string InfectedZone = "infected_zone";
}
// -------------------------------------------------------------------------
class EconomyMapStrings
{
private void EconomyMapStrings();
private void ~EconomyMapStrings();
static string Category(string category)
{
return string.Format("category:%1", category);
}
static string Tag(string tag)
{
return string.Format("tag:%1", tag);
}
const string ALL_ALL = "all:all";
const string ALL_LOOT = "all:loot";
const string ALL_VEHICLE = "all:vehicle";
const string ALL_INFECTED = "all:infected";
const string ALL_ANIMAL = "all:animal";
const string ALL_PLAYER = "all:player";
const string ALL_PROXY = "all:proxy";
const string ALL_PROXY_STATIC = "all:proxystatic";
const string ALL_PROXY_DYNAMIC = "all:proxydynamic";
const string ALL_PROXY_ABANDONED = "all:proxyabandoned";
}
// -------------------------------------------------------------------------
class EconomyOutputStrings
{
private void EconomyOutputStrings();
private void ~EconomyOutputStrings();
const string LINKS = "links";
const string SUSPICIOUS = "suspicious";
const string DE_CLOSE_POINT = "declosepoint";
const string ABANDONED = "abandoned";
const string EMPTY = "empty";
const string CLOSE = "close";
const string WORLD = "world";
const string STATUS = "status";
const string LOOT_SIZE = "lootsize";
}
// -------------------------------------------------------------------------
enum ESpawnVolumeVis
{
OFF = 0,
ADAPTIVE,
VOLUME,
OCCUPIED,
}
// -------------------------------------------------------------------------
class CEApi
{
private void CEApi() {}
private void ~CEApi() {}
proto native void ExportSpawnData();
proto native void ExportProxyData( vector vCenter = vector.Zero, float fRadius = 0 );
proto native void ExportClusterData();
proto native void ExportProxyProto();
proto native void MarkCloseProxy( float fRadius, bool bAllSelections );
proto native void RemoveCloseProxy();
proto native void ListCloseProxy( float fRadius );
proto native bool SpawnAnalyze( string sClassName );
proto native void TimeShift( float fShift );
proto native void OverrideLifeTime( float fLifeTime );
proto native Entity SpawnGroup( string sGroupName, vector vPos, float fAngle = -1 );
proto native void SpawnDE( string sEvName, vector vPos, float fAngle = -1 );
proto native void SpawnDEEx( string sEvName, vector vPos, float fAngle, int uFlags );
proto native void SpawnLoot( string sEvName, vector vPos, float fAngle, int iCount = 1, float fRange = 1 );
proto native void SpawnDynamic( vector vPos, bool bShowCylinders = true, float fDefaultDistance = 0 );
proto native void SpawnVehicles( vector vPos, bool bShowCylinders = false, float fDefaultDistance = 20 );
proto native void SpawnBuilding( vector vPos, bool bShowCylinders = false, float fDefaultDistance = 20 );
proto native void SpawnEntity( string sClassName, vector vPos, float fRange, int iCount );
proto native Object SpawnSingleEntity( string sClassName, vector vPos );
proto native void SpawnRotation( string sClassName, vector vPos, float fRange, int iCount, int iFlags );
proto native void SpawnPerfTest( string sClassName, int iCount );
proto native void CleanMap();
proto native void EconomyLog( string sLogType );
proto native void EconomyMap( string sMapType );
proto native void EconomyOutput( string sOutputType, float fRange );
proto native void RadiusLifetimeIncrease( vector vCenter, float fRadius, float fValue );
proto native void RadiusLifetimeDecrease( vector vCenter, float fRadius, float fValue );
proto native void RadiusLifetimeReset( vector vCenter, float fRadius );
proto native int GetCEGlobalInt(string varName);
proto native float GetCEGlobalFloat(string varName);
proto native string GetCEGlobalString(string varName);
proto native bool AvoidPlayer(vector vPos, float fDistance); // return false when there is a player
proto native bool AvoidVehicle(vector vPos, float fDistance, string sDEName = "");
proto native int CountPlayersWithinRange(vector vPos, float fRange);
// "Loot Spawn Edit"
//{
proto native void LootSetSpawnVolumeVisualisation(ESpawnVolumeVis mode);
proto native void LootToggleSpawnSetup(bool mode);
proto native void LootToggleVolumeEditing(bool mode);
proto native void LootRetraceGroupPoints();
proto native void LootExportGroup();
proto native void LootExportAllGroups();
proto native void LootExportMap();
proto native void LootExportClusters();
//}
// "Loot Tool"
//{
proto native void LootDepleteLifetime();
proto native void LootSetDamageToOne();
proto native void LootDepleteAndDamage();
//}
// "Infected"
//{
proto native void InfectedToggleVisualisation(bool mode);
proto native void InfectedToggleZoneInfo(bool mode);
proto native void InfectedSpawn();
proto native void InfectedResetCleanup();
//}
// "Animal"
//{
proto native void AnimalToggleVisualisation(bool mode);
proto native void AnimalSpawn();
proto native void AnimalAmbientSpawn();
//}
proto native void ToggleVehicleAndWreckVisualisation(bool mode);
proto native void ToggleLootVisualisation(bool mode);
proto native void ToggleClusterVisualisation(bool mode);
proto native void ToggleDynamicEventStatus(bool mode);
proto native void ToggleDynamicEventVisualisation(bool mode);
proto native void DynamicEventSpawn();
proto native void DynamicEventExport();
proto native void ToggleOverallStats(bool mode);
proto native void PlatformStatTest();
proto native void LootToggleProxyEditing(bool mode);
proto native void OnUpdate();
};
proto native CEApi GetCEApi();
// -------------------------------------------------------------------------
class CEItemProfile
{
private void CEItemProfile() {}
private void ~CEItemProfile() {}
proto native int GetNominal(); // nominal - how many items should be aproximately in map
proto native int GetMin(); // min - minimal count should be available in map
proto native float GetQuantityMin(); // min quantity (0.0 - 1.0) (like ammobox - this determine how many bullets are there, or water bottle)
proto native float GetQuantityMax(); // max quantity (0.0 - 1.0) (like ammobox - this determine how many bullets are there, or water bottle)
proto native float GetQuantity(); // random quantity (0.0 - 1.0)
proto native float GetLifetime(); // maximum lifetime in (seconds) - what is the idle before item abandoned at ground gets deleted
proto native float GetRestock(); // restock is oposite of lifetime - idle before item is allowed to respawn when required
proto native int GetCost(); // cost of item determines its &#39;value&#39; for players (this serve as priority during respawn and cleanup operation)
proto native int GetUsageFlags(); // area usage flags (each bit has assigned group - which as part of map overlay effectively affects spawning)
proto native int GetValueFlags(); // area value flags (each bit has assigned group - which as part of map overlay effectively affects spawning)
};