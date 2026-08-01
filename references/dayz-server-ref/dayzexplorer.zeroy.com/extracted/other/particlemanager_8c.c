// ======================================================================
// File: particlemanager_8c_source.html
// Category: other
// ======================================================================

enum ParticleManagerSettingsFlags
{
NONE,
FIXED_INDEX,
BLOCKING,
DISABLE_VIRTUAL,
REUSE_OWNED,
};
class ParticleManagerConstants
{
#ifdef BULDOZER
static const int POOL_SIZE = 1;
#else
static const int POOL_SIZE = 10000;
#endif
static const int FLAGS = ParticleManagerSettingsFlags.NONE;
}
class ParticleManagerSettings
{
void ParticleManagerSettings(int poolSize, int flags = ParticleManagerSettingsFlags.NONE)
{
}
void ~ParticleManagerSettings()
{
}
}
class ParticleManagerEvents
{
ref ScriptInvoker Event_OnAllocation = new ScriptInvoker();
ref ScriptInvoker Event_OnAllocationEnd = new ScriptInvoker();
}
class ParticleManager : Managed
{
private static ref ParticleManager g_ParticleManager;
static ParticleManager GetInstance()
{
if (!g_ParticleManager && !g_Game.IsDedicatedServer())
{
g_ParticleManager = new ParticleManager(
new ParticleManagerSettings(
ParticleManagerConstants.POOL_SIZE,
ParticleManagerConstants.FLAGS));
g_ParticleManager.SetName("GlobalParticleManager");
}
return g_ParticleManager;
}
static void CleanupInstance()
{
if (g_ParticleManager)
delete g_ParticleManager;
}
void ParticleManager(ParticleManagerSettings settings)
{
}
void ~ParticleManager()
{
}
ParticleSource CreateParticle( int id, vector pos, bool playOnCreation = false, Object parent = null, vector ori = vector.Zero, bool forceWorldRotation = false, Class owner = null )
{
int flags = ParticlePropertiesFlags.NONE;
if (playOnCreation)
{
flags = flags | ParticlePropertiesFlags.PLAY_ON_CREATION;
}
if (forceWorldRotation)
{
flags = flags | ParticlePropertiesFlags.FORCE_WORLD_ROT;
}
return CreateParticleEx(id, pos, flags, parent, ori, owner);
}
ParticleSource CreateParticleEx( int id, vector pos, int flags = ParticlePropertiesFlags.NONE, Object parent = null, vector ori = vector.Zero, Class owner = null )
{
string particlePath = ParticleList.GetParticleFullPath(id);
if (particlePath == "") // There is already an error inside of ParticleList signaling this
{
ErrorEx(string.Format("Could not create ParticleSource as particle id %1 is invalid.", id));
return null;
}
ParticleProperties props = new ParticleProperties(pos, flags, parent, ori, owner);
ParticleSource p = CreateParticleByPath(particlePath, props);
return p;
}
ParticleSource CreateOnObject(
int particle_id,
Object parent_obj,
vector local_pos = "0 0 0",
vector local_ori = "0 0 0",
bool force_world_rotation = false )
{
return CreateParticle(particle_id, local_pos, false, parent_obj, local_ori, force_world_rotation);
}
ParticleSource Create( int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0" )
{
return CreateOnObject( particle_id, parent_obj, local_pos, local_ori);
}
ParticleSource CreateInWorld( int particle_id, vector global_pos, vector global_ori = "0 0 0", bool force_world_rotation = false )
{
return CreateParticle(particle_id, global_pos, false, null, global_ori, force_world_rotation);
}
ParticleSource Create( int particle_id, vector global_pos, vector global_ori = "0 0 0" )
{
return CreateInWorld( particle_id, global_pos, global_ori );
}
ParticleSource PlayOnObject(int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_world_rotation = false )
{
return CreateParticle(particle_id, local_pos, true, parent_obj, local_ori, force_world_rotation);
}
ParticleSource Play( int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0" )
{
return PlayOnObject( particle_id, parent_obj, local_pos, local_ori);
}
ParticleSource PlayInWorld(int particle_id, vector global_pos)
{
return PlayInWorldEx(particle_id, null, global_pos);
}
ParticleSource PlayInWorldEx(int particle_id, Object parent_obj, vector global_pos, vector global_ori = "0 0 0", bool force_world_rotation = false)
{
return CreateParticle(particle_id, global_pos, true, parent_obj, global_ori, force_world_rotation);
}
ParticleSource Play( int particle_id, vector global_pos)
{
return PlayInWorld( particle_id, global_pos);
}
proto native int CreateParticles(array<ParticleSource> particles, string path, notnull ParticlePropertiesArray properties, int count = 1);
ParticleSource CreateParticleByPath(string path, notnull ParticleProperties properties)
{
array<ParticleSource> tempArr = new array<ParticleSource>;
CreateParticles(tempArr, path, {properties}, 1);
if (tempArr.Count() > 0)
return tempArr[0];
else
return null;
}
int CreateParticlesById(int id, notnull ParticlePropertiesArray properties, int count)
{
return CreateParticles(null, ParticleList.GetParticleFullPath(id), properties, count);
}
array<ParticleSource> CreateParticlesByIdArr(int id, notnull ParticlePropertiesArray properties, int count)
{
array<ParticleSource> outArr = new array<ParticleSource>;
CreateParticles(outArr, ParticleList.GetParticleFullPath(id), properties, count);
return outArr;
}
ParticleSource CreateParticleById(int id, ParticleProperties properties)
{
array<ParticleSource> tempArr = new array<ParticleSource>;
CreateParticles(tempArr, ParticleList.GetParticleFullPath(id), {properties}, 1);
if (tempArr.Count() > 0)
return tempArr[0];
else
return null;
}
proto native int PlayParticles(out array<ParticleSource> particles, string path, notnull array<vector> positions, int count = 1);
array<ParticleSource> PlayParticlesById(int id, array<vector> positions, int count)
{
array<ParticleSource> outArr = new array<ParticleSource>;
PlayParticles(outArr, ParticleList.GetParticleFullPath(id), positions, count);
return outArr;
}
ParticleSource PlayParticleById(int id, array<vector> position)
{
array<ParticleSource> tempArr = new array<ParticleSource>;
PlayParticles(tempArr, ParticleList.GetParticleFullPath(id), position, 1);
if (tempArr.Count() > 0)
return tempArr[0];
else
return null;
}
proto native ParticleSource GetParticle(int index);
proto native int GetParticles(out array<ParticleSource> outArray, int startIndex, int count);
array<ParticleSource> GetParticlesEx(int startIndex, int count)
{
array<ParticleSource> outArr = new array<ParticleSource>;
GetParticles(outArr, startIndex, count);
return outArr;
}
proto native void SetName(string name);
proto string GetName();
proto string GetDebugNameNative();
override string GetDebugName()
{
return GetDebugNameNative();
}
proto int GetCountID();
proto native static int GetStaticCount();
proto native static int GetStaticActiveCount();
proto native int GetPoolSize();
proto native int GetAllocatedCount();
proto native int GetVirtualCount();
proto native int GetPlayingCount();
proto native bool IsFinishedAllocating();
private proto void SetScriptEvents(Managed events);
private proto Managed GetScriptEvents();
ParticleManagerEvents GetEvents()
{
return ParticleManagerEvents.Cast(GetScriptEvents());
}
void OnAllocation(array<ParticleSource> allocatedParticles)
{
GetEvents().Event_OnAllocation.Invoke(this, allocatedParticles);
}
void OnAllocationEnd()
{
GetEvents().Event_OnAllocationEnd.Invoke(this);
}
}