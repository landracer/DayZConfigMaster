// ======================================================================
// File: particlesource_8c_source.html
// Category: other
// ======================================================================

enum ParticleAutoDestroyFlags
{
NONE,
ON_END,
ON_STOP,
ALL,
}
enum PlayParticleFlags
{
NONE,
// Is just a placeholder for now
}
enum StopParticleFlags
{
NONE,
RESET,
IMMEDIATE,
VISIBLE,
PAUSE,
}
enum EGetParticleMode
{
FULL,
NO_EXT,
FILE,
}
enum ParticlePropertiesFlags
{
NONE,
PLAY_ON_CREATION,
FORCE_WORLD_ROT,
KEEP_PARENT_ON_END,
};
class ParticleProperties
{
void ParticleProperties(vector localPos, int flags, Object parent = null, vector localOri = vector.Zero, Class owner = null)
{
}
void ~ParticleProperties()
{
}
proto Object GetParent();
proto Class GetOwner();
proto vector GetLocalPos();
proto vector GetLocalOri();
proto int GetPPFlags();
proto bool IsPlayOnCreation();
proto bool IsForceWorldRotation();
proto bool IsKeepParentOnEnd();
}
typedef array<ref ParticleProperties> ParticlePropertiesArray;
typedef array<ParticleSource> ParticleSourceArray;
class ParticleSource : Particle
{
void ParticleSource()
{
m_ParticleEffect = this;
}
void ~ParticleSource()
{
}
override protected void ParticleInit() {}
proto native void SetParticleAutoDestroyFlags(ParticleAutoDestroyFlags flags);
void DisableAutoDestroy()
{
SetParticleAutoDestroyFlags(ParticleAutoDestroyFlags.NONE);
}
proto native int GetParticleAutoDestroyFlags();
static ParticleSource CreateParticle( int id, vector pos, bool playOnCreation = false, Object parent = null, vector ori = vector.Zero, bool forceWorldRotation = false, Class owner = null )
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
static ParticleSource CreateParticleEx( int id, vector pos, int flags = ParticlePropertiesFlags.NONE, Object parent = null, vector ori = vector.Zero, Class owner = null )
{
string particlePath = ParticleList.GetParticleFullPath(id);
if (particlePath == "") // There is already an error inside of ParticleList signaling this
{
ErrorEx(string.Format("Could not create ParticleSource as particle id %1 is invalid.", id));
return null;
}
vector localPos = pos;
if (parent)
pos = parent.GetPosition();
ParticleSource p = ParticleSource.Cast( g_Game.CreateObjectEx("ParticleSource", pos, ECE_LOCAL) );
p.SetParticle(particlePath);
ParticleProperties props = new ParticleProperties(localPos, flags, parent, ori, owner);
p.ApplyProperties(props);
return p;
}
override static Particle CreateOnObject(
int particle_id,
Object parent_obj,
vector local_pos = "0 0 0",
vector local_ori = "0 0 0",
bool force_world_rotation = false )
{
return CreateParticle(particle_id, local_pos, false, parent_obj, local_ori, force_world_rotation);
}
override static Particle Create( int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0" )
{
return CreateOnObject( particle_id, parent_obj, local_pos, local_ori);
}
override static Particle CreateInWorld( int particle_id, vector global_pos, vector global_ori = "0 0 0", bool force_world_rotation = false )
{
return CreateParticle(particle_id, global_pos, false, null, global_ori, force_world_rotation);
}
override static ParticleSource Create( int particle_id, vector global_pos, vector global_ori = "0 0 0" )
{
return CreateInWorld( particle_id, global_pos, global_ori );
}
override static Particle PlayOnObject(int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_world_rotation = false )
{
return CreateParticle(particle_id, local_pos, true, parent_obj, local_ori, force_world_rotation);
}
override static Particle Play( int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0" )
{
return PlayOnObject( particle_id, parent_obj, local_pos, local_ori);
}
override static Particle PlayInWorld( int particle_id, vector global_pos)
{
return CreateParticle(particle_id, global_pos, true);
}
override static Particle Play( int particle_id, vector global_pos)
{
return PlayInWorld( particle_id, global_pos);
}
private proto bool PlayParticleNative(int flags);
override bool PlayParticleEx(int particle_id = -1, int flags = 0)
{
if ( particle_id > -1 )
{
// Here we can just do it directly
// While with the old system it will not work when the particle is already created
SetParticleByID(particle_id);
}
return PlayParticleNative(flags);
}
private proto bool StopParticleNative(int flags);
override bool StopParticle(int flags = 0)
{
return StopParticleNative(flags);
}
private proto native bool ResetParticleNative();
override bool ResetParticle()
{
return ResetParticleNative();
}
private proto native bool RestartParticleNative();
override bool RestartParticle()
{
return RestartParticleNative();
}
private proto bool IsParticlePlayingNative();
override bool IsParticlePlaying()
{
return IsParticlePlayingNative();
}
private proto native bool SetParticleNative(string path);
private bool SetParticle(string path)
{
return SetParticleNative(path);
}
bool SetParticleByID(int id)
{
return SetParticle(ParticleList.GetParticleFullPath(id));
}
override void SetSource(int particle_id)
{
SetParticleByID(particle_id);
}
private proto bool GetParticleNative(out string path, EGetParticleMode mode);
bool GetParticle(out string path, EGetParticleMode mode)
{
return GetParticleNative(path, mode);
}
override int GetParticleID()
{
string path;
if (GetParticle(path, EGetParticleMode.FILE))
return ParticleList.GetParticleIDByName(path);
else
return -1;
}
int GetParticleIDLegacy()
{
string path;
if (GetParticle(path, EGetParticleMode.NO_EXT))
return ParticleList.GetParticleID(path);
else
return -1;
}
private proto native bool ApplyPropertiesNative(ParticleProperties properties);
bool ApplyProperties(ParticleProperties properties)
{
return ApplyPropertiesNative(properties);
}
override Object GetDirectParticleEffect()
{
return this;
}
override Object GetParticleParent()
{
return Object.Cast(GetParent());
}
private proto bool HasActiveParticleNative();
override bool HasActiveParticle()
{
return HasActiveParticleNative();
}
private proto int GetParticleCountNative();
override int GetParticleCount()
{
return GetParticleCountNative();
}
private proto bool IsRepeatNative();
override bool IsRepeat()
{
return IsRepeatNative();
}
private proto float GetMaxLifetimeNative();
override float GetMaxLifetime()
{
return GetMaxLifetimeNative();
}
proto native Class GetOwner();
proto native void SetOwner(Class owner);
proto native void Orphan();
proto native ParticleManager GetParticleManager();
proto native int GetIndex();
proto int GetCountID();
proto native static int GetStaticCount();
proto native static int GetStaticActiveCount();
override protected void OnParticleParented(IEntity parent)
{
m_ParentObject = Object.Cast(parent);
super.OnParticleParented(parent);
}
override protected void OnParticleUnParented(IEntity parent)
{
m_ParentObject = null;
// Since we have lost the parent, we will need to refresh the wiggle
// As it uses a cached local position, which is now no longer correct
if (!m_WiggleProcessing && IsWiggling())
{
float randomAngle = m_MaxOriWiggle;
float randomInterval = m_MaxOriInterval;
m_DefaultPos = m_DefaultWorldPos;
m_DefaultOri = m_DefaultWorldOri;
StopWiggle();
SetWiggle(randomAngle, randomInterval);
}
super.OnParticleUnParented(parent);
}
override protected void OnParticleStop()
{
if (IsWiggling())
{
StopWiggle();
delete m_RandomizeOri;
}
super.OnParticleStop();
}
override void AddAsChild(Object parent, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_rotation_to_world = false)
{
int flags = ParticlePropertiesFlags.NONE;
if (force_rotation_to_world)
flags = ParticlePropertiesFlags.FORCE_WORLD_ROT;
ParticleProperties props = new ParticleProperties(local_pos, flags, parent, local_ori);
ApplyProperties(props);
}
override void SetParticleParam(int parameter_id, float value )
{
SetParticleParm(this, -1, parameter_id, value);
}
override void SetParameter(int emitter, int parameter, float value)
{
SetParticleParm(this, emitter, parameter, value);
}
override void GetParameter(int emitter, int parameter, out float value)
{
GetParticleParm(this, emitter, parameter, value);
}
override float GetParameterEx(int emitter, int parameter)
{
float value;
GetParticleParm(this, emitter, parameter, value);
return value;
}
override void ScaleParticleParamFromOriginal(int parameter_id, float coef )
{
int emitors = GetParticleEmitorCount(this);
for (int i = 0; i < emitors; ++i)
{
float value;
GetParticleParmOriginal(this, i, parameter_id, value);
SetParticleParm(this, i, parameter_id, value * coef);
}
}
override void ScaleParticleParam(int parameter_id, float coef )
{
int emitors = GetParticleEmitorCount(this);
for (int i = 0; i < emitors; ++i)
{
float value;
GetParticleParm(this, i, parameter_id, value);
SetParticleParm(this, i, parameter_id, value * coef);
}
}
override void IncrementParticleParamFromOriginal(int parameter_id, float value )
{
int emitors = GetParticleEmitorCount(this);
for (int i = 0; i < emitors; ++i)
{
float param;
GetParticleParmOriginal(this, i, parameter_id, param);
SetParticleParm(this, i, parameter_id, param + value);
}
}
override void IncrementParticleParam(int parameter_id, float value )
{
int emitors = GetParticleEmitorCount(this);
for (int i = 0; i < emitors; ++i)
{
float param;
GetParticleParm(this, i, parameter_id, param);
SetParticleParm(this, i, parameter_id, param + value);
}
}
override void SetWiggle(float random_angle, float random_interval)
{
if (random_angle != 0 || random_interval != 0)
{
if (IsWiggling())
{
m_MaxOriWiggle = random_angle;
m_MaxOriInterval = random_interval;
return;
}
// We need the position to be accurate before storing it
Update();
// These are only ever used within the Wiggle API
// To restore the properties after wiggling
// So let&#39;s only set them within the Wiggle API c:
m_DefaultPos = GetLocalPosition();
m_DefaultOri = GetLocalYawPitchRoll();
m_DefaultWorldPos = GetWorldPosition();
m_DefaultWorldOri = GetYawPitchRoll();
m_ForceOrientationRelativeToWorld = IsHierarchyPositionOnly();
}
super.SetWiggle(random_angle, random_interval);
}
override void StopWiggle()
{
bool wiggling = IsWiggling();
super.StopWiggle();
if (wiggling)
{
// Restore pre-wiggle orientation
int flags = ParticlePropertiesFlags.NONE;
if (m_ForceOrientationRelativeToWorld)
flags = ParticlePropertiesFlags.FORCE_WORLD_ROT;
ParticleProperties prop = new ParticleProperties(m_DefaultPos, flags, GetParticleParent(), m_DefaultOri, GetOwner());
ApplyProperties(prop);
}
}
override private void RandomizeOrientation()
{
if (ToDelete())
return;
m_WiggleProcessing = true;
if ( !m_RandomizeOri.IsRunning() )
m_RandomizeOri.Run( Math.RandomFloat(0, m_MaxOriInterval) , this, "RandomizeOrientation", null, false);
int flags = ParticlePropertiesFlags.NONE;
if (m_ForceOrientationRelativeToWorld)
flags = ParticlePropertiesFlags.FORCE_WORLD_ROT;
ParticleProperties prop = new ParticleProperties(m_DefaultPos, flags, GetParticleParent(), m_DefaultOri + RandWiggleVector(), GetOwner());
ApplyProperties(prop);
m_WiggleProcessing = false;
}
override private void UpdateState() { ErrorEx("Should not be in use on ParticleSource."); }
override private void DestroyParticleEffect() { ErrorEx("Should not be in use on ParticleSource."); }
override private void CreateParticleEffect() { ErrorEx("Should not be in use on ParticleSource."); }
override protected void EOnFrame(IEntity other, float timeSlice) { ErrorEx("Should not be in use on ParticleSource."); }
override private void OnCheckAutoDelete() { ErrorEx("Should not be in use on ParticleSource."); }
override private void OnToDelete() { ErrorEx("Should not be in use on ParticleSource."); }
}