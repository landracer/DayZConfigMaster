// ======================================================================
// File: particle_8c_source.html
// Category: other
// ======================================================================

class Particle : ParticleBase
{
protected int m_ParticleID;
protected float m_Lifetime;
protected bool m_IsRepeat;
private bool m_MarkedForDeletion;
bool m_WiggleProcessing;
bool m_ForceOrientationRelativeToWorld;
vector m_DefaultOri;
vector m_DefaultPos;
vector m_DefaultWorldOri;
vector m_DefaultWorldPos;
float m_MaxOriWiggle;
float m_MaxOriInterval;
ref Timer m_RandomizeOri;
protected Object m_ParentObject;
protected Object m_ParticleEffect;
protected int m_PreviousFrame;
private vector m_GlobalPosPreviousFrame;
static private const int MAX_EMITORS = 30;
void Particle()
{
ParticleInit();
}
protected void ParticleInit()
{
SetFlags(EntityFlags.VISIBLE, true);
SetEventMask(EntityEvent.INIT);
SetEventMask(EntityEvent.FRAME);
}
static Particle CreateOnObject( int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_world_rotation = false )
{
if (!parent_obj)
Error("ERROR when creating a particle! Parameter parent_obj is NULL!");
vector global_pos = parent_obj.GetPosition();
Particle p = CreateInWorld(particle_id, global_pos, Vector(0,0,0), force_world_rotation);
p.AddAsChild(parent_obj, local_pos, local_ori, force_world_rotation);
p.m_DefaultOri = local_ori;
return p;
}
static Particle Create( int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0" )
{
return CreateOnObject( particle_id, parent_obj, local_pos, local_ori);
}
static Particle CreateInWorld( int particle_id, vector global_pos, vector global_ori = "0 0 0", bool force_world_rotation = false )
{
Particle p = Particle.Cast( g_Game.CreateObjectEx("Particle", global_pos, ECE_LOCAL) );
p.SetSource(particle_id);
p.SetOrientation(global_ori);
p.m_ForceOrientationRelativeToWorld = force_world_rotation;
return p;
}
static Particle Create( int particle_id, vector global_pos, vector global_ori = "0 0 0" )
{
return CreateInWorld( particle_id, global_pos, global_ori );
}
static Particle PlayOnObject( int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_world_rotation = false )
{
Particle p = CreateOnObject(particle_id, parent_obj, local_pos, local_ori, force_world_rotation);
p.PlayParticle();
return p;
}
static Particle Play( int particle_id, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0" )
{
return PlayOnObject( particle_id, parent_obj, local_pos, local_ori);
}
static Particle PlayInWorld( int particle_id, vector global_pos)
{
Particle p = CreateInWorld(particle_id, global_pos);
p.PlayParticle();
return p;
}
static Particle Play( int particle_id, vector global_pos)
{
return PlayInWorld( particle_id, global_pos);
}
override void PlayParticle(int particle_id = -1)
{
PlayParticleEx(particle_id, 0);
}
override bool PlayParticleEx(int particle_id = -1, int flags = 0)
{
if ( particle_id > -1 )
{
SetSource(particle_id);
}
OnParticleStart();
UpdateState();
return true;
}
void Play(int particle_id = -1)
{
PlayParticle(particle_id);
}
override bool StopParticle(int flags = 0)
{
OnParticleStop();
// Without the following we might get an error when a particle parent is despawned client-side.
Object parent = Object.Cast( GetParent() );
if ( parent && !ToDelete())
{
vector world_pos = GetPosition();
parent.RemoveChild(this);
SetPosition(world_pos);
}
UpdateState();
return true;
}
void Stop()
{
StopParticle();
}
void SetSource(int particle_id)
{
m_ParticleID = particle_id;
}
int GetParticleID()
{
return m_ParticleID;
}
Object GetDirectParticleEffect()
{
return m_ParticleEffect;
}
Object GetParticleParent()
{
return m_ParentObject;
}
bool HasActiveParticle()
{
if (m_ParticleEffect)
{
return ParticleHasActive(m_ParticleEffect);
}
return false;
}
int GetParticleCount()
{
if (m_ParticleEffect)
{
return ParticleGetCount(m_ParticleEffect);
}
return 0;
}
bool IsRepeat()
{
if (m_ParticleEffect)
{
bool repeat = false;
int emitors = GetParticleEmitorCount(m_ParticleEffect);
for (int i = 0; i < emitors; ++i)
{
GetParticleParm(m_ParticleEffect, i, EmitorParam.REPEAT, repeat);
if (repeat)
{
return true;
}
}
}
return false;
}
float GetMaxLifetime()
{
float lifetime_return = 0;
if (m_ParticleEffect)
{
float lifetime_min = 0;
float lifetime_random = 0;
float effect_time = 0;
float lifetime_sum = 0;
int emitors = GetParticleEmitorCount(m_ParticleEffect);
for (int i = 0; i < emitors; ++i)
{
GetParticleParm(m_ParticleEffect, i, EmitorParam.LIFETIME, lifetime_min);
GetParticleParm(m_ParticleEffect, i, EmitorParam.LIFETIME_RND, lifetime_random);
GetParticleParm(m_ParticleEffect, i, EmitorParam.EFFECT_TIME, effect_time);
lifetime_sum = lifetime_min + lifetime_random + effect_time;
if ( lifetime_sum > lifetime_return )
{
lifetime_return = lifetime_sum;
}
}
}
return lifetime_return;
}
protected void UpdateState()
{
if ( m_IsPlaying == false && m_ParticleEffect)
{
DestroyParticleEffect();
}
else if ( m_IsPlaying == true && m_ParticleEffect == null )
{
CreateParticleEffect();
}
}
private void CreateParticleEffect()
{
if ( !g_Game.IsServer() || !g_Game.IsMultiplayer() )
{
string fullPath = ParticleList.GetParticleFullPath(m_ParticleID);
if (fullPath == "")
{
ErrorEx("Could not play Particle as there is no valid particle id assigned.");
m_IsPlaying = false;
return;
}
if ( m_ParticleEffect == null )
{
m_ParticleEffect = g_Game.CreateObjectEx("#particlesourceenf", vector.Zero, ECE_LOCAL); // particle source must be lowercase!
}
AddChild(m_ParticleEffect, -1, m_ForceOrientationRelativeToWorld);
vobject vobj = GetObject( fullPath );
m_ParticleEffect.SetObject(vobj, "");
ReleaseObject(vobj);
m_IsRepeat = IsRepeat();
m_Lifetime = GetMaxLifetime();
}
}
private void DestroyParticleEffect()
{
if ( m_ParticleEffect && g_Game )
{
SetParameter(-1, EmitorParam.LIFETIME, 0);
SetParameter(-1, EmitorParam.LIFETIME_RND, 0);
SetParameter(-1, EmitorParam.REPEAT, 0);
m_IsRepeat = false;
}
}
override void EOnFrame(IEntity other, float timeSlice)
{
m_Lifetime -= timeSlice;
OnCheckAutoDelete();
}
void OnCheckAutoDelete()
{
if (m_Lifetime <= 0)
{
if (!m_MarkedForDeletion)
{
m_IsRepeat = IsRepeat(); // It is possible that the REPEAT flag was changed during lifetime, so it needs to be checked again.
if ( m_IsRepeat )
{
m_Lifetime = GetMaxLifetime();
}
else
{
OnParticleStop();
if ( GetParticleCount() == 0 )
{
m_MarkedForDeletion = true;
OnToDelete();
OnParticleEnd();
}
}
}
else
{
if ( m_MarkedForDeletion )
{
if (m_ParticleEffect)
{
m_ParticleEffect.Delete();
m_ParticleEffect = null;
}
Delete();
}
}
}
}
private void OnToDelete()
{
}
void AddAsChild(Object parent, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_rotation_to_world = false)
{
if (ToDelete())
return;
if (parent)
{
// AddAsChild method is sometimes called from a timer.
// Due to that it is necesarry to use ToDelete() here to check if the parent object is flagged for deletion or not on client,
// because sometimes this code is executed before the parent&#39;s destructor from where this would normally be handled.
if (!parent.ToDelete())
{
SetPosition(local_pos);
SetOrientation(local_ori);
m_ParentObject = parent;
m_DefaultPos = local_pos;
m_ForceOrientationRelativeToWorld = force_rotation_to_world;
if (m_ParticleEffect)
AddChild(m_ParticleEffect, -1, m_ForceOrientationRelativeToWorld);
parent.AddChild(this, -1, false);
}
}
else
{
if (m_ParentObject && !m_ParentObject.ToDelete())
{
m_ParentObject.RemoveChild(this, true);
m_ParentObject = null;
}
}
}
void SetParticleParam(int parameter_id, float value )
{
if (!m_ParticleEffect)
return;
SetParticleParm(m_ParticleEffect, -1, parameter_id, value);
}
void SetParameter(int emitter, int parameter, float value)
{
if (!m_ParticleEffect)
return;
SetParticleParm(m_ParticleEffect, emitter, parameter, value);
}
void GetParameter(int emitter, int parameter, out float value)
{
if (!m_ParticleEffect)
return;
GetParticleParm(m_ParticleEffect, emitter, parameter, value);
}
float GetParameterEx(int emitter, int parameter)
{
if (!m_ParticleEffect)
return 0;
float value;
GetParticleParm(m_ParticleEffect, emitter, parameter, value);
return value;
}
float GetParameterOriginal(int emitter, int parameter)
{
if (!m_ParticleEffect)
return 0;
float value;
GetParticleParmOriginal(m_ParticleEffect, emitter, parameter, value);
return value;
}
void ScaleParticleParamFromOriginal(int parameter_id, float coef )
{
if (!m_ParticleEffect)
return;
int emitors = GetParticleEmitorCount(m_ParticleEffect);
for (int i = 0; i < emitors; ++i)
{
float value;
GetParticleParmOriginal(m_ParticleEffect, i, parameter_id, value);
SetParticleParm(m_ParticleEffect, i, parameter_id, value * coef);
}
}
void ScaleParticleParam(int parameter_id, float coef )
{
if (!m_ParticleEffect)
return;
int emitors = GetParticleEmitorCount(m_ParticleEffect);
for (int i = 0; i < emitors; ++i)
{
float value;
GetParticleParm(m_ParticleEffect, i, parameter_id, value);
SetParticleParm(m_ParticleEffect, i, parameter_id, value * coef);
}
}
void IncrementParticleParamFromOriginal(int parameter_id, float value )
{
if (!m_ParticleEffect)
return;
int emitors = GetParticleEmitorCount(m_ParticleEffect);
for (int i = 0; i < emitors; ++i)
{
float param;
GetParticleParmOriginal(m_ParticleEffect, i, parameter_id, param);
SetParticleParm(m_ParticleEffect, i, parameter_id, param + value);
}
}
void IncrementParticleParam(int parameter_id, float value )
{
if (!m_ParticleEffect)
return;
int emitors = GetParticleEmitorCount(m_ParticleEffect);
for (int i = 0; i < emitors; ++i)
{
float param;
GetParticleParm(m_ParticleEffect, i, parameter_id, param);
SetParticleParm(m_ParticleEffect, i, parameter_id, param + value);
}
}
bool IsWiggling()
{
return m_RandomizeOri && m_RandomizeOri.IsRunning();
}
void SetWiggle(float random_angle, float random_interval)
{
if ( random_angle != 0 || random_interval != 0 )
{
m_MaxOriWiggle = random_angle;
m_MaxOriInterval = random_interval;
if ( !m_RandomizeOri )
m_RandomizeOri = new Timer( CALL_CATEGORY_GAMEPLAY );
if ( !m_RandomizeOri.IsRunning() ) // Makes sure the timer is NOT running already
m_RandomizeOri.Run( Math.RandomFloat(0, m_MaxOriInterval) , this, "RandomizeOrientation", null, false);
}
else
{
StopWiggle();
}
}
void StopWiggle()
{
if ( m_RandomizeOri )
{
m_RandomizeOri.Stop();
}
m_MaxOriWiggle = 0;
m_MaxOriInterval = 0;
}
void RandomizeOrientation()
{
m_WiggleProcessing = true;
if (m_ParentObject)
{
if ( !m_RandomizeOri.IsRunning() )
{
m_RandomizeOri.Run( Math.RandomFloat(0, m_MaxOriInterval) , this, "RandomizeOrientation", NULL, false);
}
Object old_parent = m_ParentObject;
AddAsChild( null );
AddAsChild( old_parent, m_DefaultPos, m_DefaultOri + RandWiggleVector() );
}
m_WiggleProcessing = false;
}
protected vector RandWiggleVector()
{
return Vector( RandWiggleFloat(), RandWiggleFloat(), RandWiggleFloat() );
}
protected float RandWiggleFloat()
{
return Math.RandomFloatInclusive(-m_MaxOriWiggle, m_MaxOriWiggle);
}
}