// Source: F:/Games/Dayz/scripts/3_game/ce/centraleconomy.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
// -------------------------------------------------------------------------
// object (SF) Spawn Flags (use to setup behavior and/ or trigger functionality)
//
const int ECE_NONE                          = 0;
    
const int ECE_SETUP                         = 2;    // process full entity setup (when creating NEW entity)
const int ECE_TRACE                         = 4;    // trace under entity when being placed (from the point of creation)
const int ECE_CENTER                        = 8;    // use center from shape (model) for placement
 
const int ECE_UPDATEPATHGRAPH               = 32;   // update navmesh when object placed upon it
 
const int ECE_ROTATIONFLAGS                 = 512;  // enable rotation flags for object placement
const int ECE_CREATEPHYSICS                 = 1024; // create collision envelope and related physics data (if object has them)
const int ECE_INITAI                        = 2048; // init ai
const int ECE_AIRBORNE                      = 4096; // create flying unit in the air
 
const int ECE_EQUIP_ATTACHMENTS             = 8192;     // equip with configured ATTACHMENTS
const int ECE_EQUIP_CARGO                   = 16384;    // equip with configured CARGO
const int ECE_EQUIP                         = 24576;    // equip with configured (ATTACHMENTS + CARGO)
const int ECE_EQUIP_CONTAINER               = 2097152;  // populate DE/ group CONTAINER during spawn
const int ECE_LOCAL                         = 1073741824; // create object locally
 
const int ECE_NOSURFACEALIGN                = 262144;   // do not align object on surface/ terrain
const int ECE_KEEPHEIGHT                    = 524288;   // keep height when creating object (do not use trace or placement on surface)
 
const int ECE_NOLIFETIME                    = 4194304;  // do not set lifetime when creating the object
const int ECE_NOPERSISTENCY_WORLD           = 8388608;  // do not save this object in world
const int ECE_NOPERSISTENCY_CHAR            = 16777216; // do not save this object in character
const int ECE_DYNAMIC_PERSISTENCY           = 33554432; // spawns in without persistency, once player takes it, persistency will be enabled if available
 
// note: use predefined combination when not solving something specific
//
const int ECE_IN_INVENTORY                  = 787456;   // ECE_CREATEPHYSICS|ECE_KEEPHEIGHT|ECE_NOSURFACEALIGN
const int ECE_PLACE_ON_SURFACE              = 1060;     // ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_TRACE
const int ECE_OBJECT_SWAP                   = 787488;   // ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_KEEPHEIGHT|ECE_NOSURFACEALIGN
 
const int ECE_FULL                          = 25126;    // ECE_SETUP|ECE_TRACE|ECE_ROTATIONFLAGS|ECE_UPDATEPATHGRAPH|ECE_EQUIP
 
// -------------------------------------------------------------------------
// object (RF) Rotation Flags (use to force and/ or invoke placement rotation)
//
const int   RF_NONE                 = 0;
 
const int   RF_FRONT                = 1;    // front side placement
const int   RF_TOP                  = 2;    // top side placement
const int   RF_LEFT                 = 4;    // left side placement
const int   RF_RIGHT                = 8;    // right side placement
const int   RF_BACK                 = 16;   // back side placement
const int   RF_BOTTOM               = 32;   // bottom side placement
 
const int   RF_ALL                  = 63;   // RF_FRONT|RF_TOP|RF_LEFT|RF_RIGHT|RF_BACK|RF_BOTTOM
 
const int   RF_IGNORE               = 64;   // ignore placement RF flags - object will spawn as model was created
 
const int   RF_TOPBOTTOM            = 34;   // RF_TOP|RF_BOTTOM
const int   RF_LEFTRIGHT            = 12;   // RF_LEFT|RF_RIGHT
const int   RF_FRONTBACK            = 17;   // RF_FRONT|RF_BACK
 
const int   RF_RANDOMROT            = 64;   // allow random rotation around axis when placing
const int   RF_ORIGINAL             = 128;  // use default placement setuped on object in config
const int   RF_DECORRECTION         = 256;  // angle correction when spawning InventoryItem at Building angle
const int   RF_DEFAULT              = 512;  // use default placement setuped on object in config
 
// -------------------------------------------------------------------------
class EconomyLogCategories
{
    private void EconomyLogCategories();
    private void ~EconomyLogCategories();
    
    const string Economy                = &quot;economy&quot;;
    const string EconomyRespawn         = &quot;economy_respawn&quot;;
    const string RespawnQueue           = &quot;respawn_queue&quot;;
    const string Container              = &quot;container&quot;;
    const string Matrix                 = &quot;matrix&quot;;
    const string UniqueLoot             = &quot;uniqueloot&quot;;
    const string Bind                   = &quot;bind&quot;;
    const string SetupFail              = &quot;setupfail&quot;;
    const string Storage                = &quot;storage&quot;;
    const string Classes                = &quot;class&quot;;
    const string Category               = &quot;category&quot;;
    const string Tag                    = &quot;tag&quot;;
    const string SCategory              = &quot;s_category&quot;;
    const string STag                   = &quot;s_tag&quot;;
    const string SAreaflags             = &quot;s_areaflags&quot;;
    const string SCrafted               = &quot;s_crafted&quot;;
    const string MapGroup               = &quot;map_group&quot;;
    const string MapComplete            = &quot;map_complete&quot;;
    const string InfectedZone           = &quot;infected_zone&quot;;
