// ======================================================================
// File: effectmanager_8c_source.html
// Category: other
// ======================================================================

class SEffectManager
{
protected static ref map<int, ref Effect> m_EffectsMap;
protected static ref array<int> m_FreeEffectIDs;
protected static int m_HighestFreeEffectID = 1;
static const int INVALID_ID = 0;
protected static bool m_IsCleanup;
protected static bool m_IsInitialized;
protected static ref map<string, ref SoundParams> m_ParamsMap;
static ref ScriptInvoker Event_OnFrameUpdate;
protected static ref map<int, EffecterBase> m_EffectersMap;
protected static ref array<int> m_FreeEffecterIDs;
protected static int m_HighestFreeEffecterID = 1;
static int PlayInWorld(notnull Effect eff, vector pos)
{
// Stop the effect first, just in case
eff.Stop();
int id = EffectRegister(eff);
eff.SetPosition( pos );
eff.Start();
return id;
}
static int PlayOnObject(notnull Effect eff, Object obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_rotation_relative_to_world = false)
{
// Stop the effect first, just in case
eff.Stop();
int id = EffectRegister(eff);
if (!obj)
{
ErrorEx("Parent object is null.", ErrorExSeverity.WARNING);
eff.SetPosition(local_pos);
}
else
{
eff.SetPosition(obj.GetPosition());
}
eff.SetParent(obj);
eff.SetLocalPosition(local_pos);
eff.SetAttachedLocalOri(local_ori);
if (force_rotation_relative_to_world)
{
EffectParticle eff_particle = EffectParticle.Cast(eff);
if (eff_particle)
{
eff_particle.ForceParticleRotationRelativeToWorld(force_rotation_relative_to_world);
}
}
eff.Start();
return id;
}
static void Stop(int effect_id)
{
Effect eff = m_EffectsMap.Get(effect_id);
if (eff)
{
eff.Stop();
}
else
{
ErrorEx(string.Format("Failed to stop Effect with ID %1. The ID is not registered in m_EffectsMap!", effect_id));
}
}
static EffectSound CreateSound(string sound_set, vector position, float play_fade_in = 0, float stop_fade_out = 0, bool loop = false, bool enviroment = false)
{
EffectSound effect_sound = new EffectSound();
effect_sound.SetSoundSet(sound_set);
effect_sound.SetPosition(position);
effect_sound.SetSoundFadeIn(play_fade_in);
effect_sound.SetSoundFadeOut(stop_fade_out);
effect_sound.SetSoundLoop(loop);
effect_sound.SetEnviromentVariables(enviroment);
EffectRegister( effect_sound );
return effect_sound;
}
static EffectSound PlaySound(string sound_set, vector position, float play_fade_in = 0, float stop_fade_out = 0, bool loop = false)
{
EffectSound effect_sound = CreateSound(sound_set, position, play_fade_in, stop_fade_out, loop, false);
effect_sound.SoundPlay();
return effect_sound;
}
static EffectSound PlaySoundParams(notnull SoundParams params, vector position, float play_fade_in = 0, float stop_fade_out = 0, bool loop = false)
{
EffectSound effect_sound = CreateSound(params.GetName(), position, play_fade_in, stop_fade_out, loop, false);
effect_sound.SoundPlayEx(params);
return effect_sound;
}
static EffectSound PlaySoundCachedParams(string sound_set, vector position, float play_fade_in = 0, float stop_fade_out = 0, bool loop = false)
{
SoundParams params = GetCachedSoundParam(sound_set);
EffectSound effect_sound = CreateSound(params.GetName(), position, play_fade_in, stop_fade_out, loop, false);
effect_sound.SoundPlayEx(params);
return effect_sound;
}
static EffectSound PlaySoundEnviroment(string sound_set, vector position, float play_fade_in = 0, float stop_fade_out = 0, bool loop = false)
{
EffectSound effect_sound = CreateSound(sound_set, position, play_fade_in, stop_fade_out, loop, true);
effect_sound.SoundPlay();
return effect_sound;
}
static EffectSound PlaySoundOnObject(string sound_set, Object parent_object, float play_fade_in = 0, float stop_fade_out = 0, bool loop = false)
{
EffectSound effect_sound = CreateSound(sound_set, parent_object.GetPosition(), play_fade_in, stop_fade_out, loop);
effect_sound.SetParent( parent_object );
effect_sound.SetLocalPosition( vector.Zero );
effect_sound.SoundPlay();
return effect_sound;
}
static void DestroyEffect(Effect effect)
{
if (effect)
{
if (effect.CanDestroy())
{
// Functionality already happens in dtor of Effect to be safe
delete effect;
}
else
{
// Make it clean up itself when done
effect.SetAutodestroy(true);
effect.Stop();
}
}
}
static bool IsEffectExist( int effect_id )
{
if (!m_IsCleanup)
return m_EffectsMap[effect_id] != null;
else
return false;
}
static Effect GetEffectByID(int effect_id)
{
if (!m_IsCleanup)
return m_EffectsMap[effect_id];
else
return null;
}
static int EffectRegister(Effect effect)
{
if (effect.IsRegistered())
{
ErrorEx(string.Format("Attempted to register Effect &#39;%1&#39; which was already registered.", effect.GetDebugName()), ErrorExSeverity.INFO);
return effect.GetID();
}
int id;
if (!m_IsCleanup)
{
id = GetFreeEffectID();
m_EffectsMap.Insert(id, effect);
effect.Event_OnRegistered(id);
}
else
ErrorEx("Attempted to register Effect while SEffectManager is cleaning up, request ignored.", ErrorExSeverity.WARNING);
return id;
}
protected static int GetFreeEffecterID()
{
int return_id;
if (m_FreeEffecterIDs.Count() > 0)
{
return_id = m_FreeEffecterIDs.Get(0);
m_FreeEffecterIDs.Remove(0);
}
else
{
return_id = m_HighestFreeEffecterID;
++m_HighestFreeEffecterID;
}
return return_id;
}
static void EffectUnregister(int id)
{
if (m_IsCleanup)
return; // No error needed, since it will have been unregistered anyways after cleanup is finished
Effect effect;
if ( m_EffectsMap.Find(id, effect) )
{
effect.Event_OnUnregistered();
m_EffectsMap.Remove(id);
}
if ( m_FreeEffectIDs.Find(id) == -1 )
{
m_FreeEffectIDs.Insert(id);
}
}
static void EffectUnregisterEx(Effect effect)
{
EffectUnregister(effect.GetID());
}
protected static int GetFreeEffectID()
{
int return_id;
if (m_FreeEffectIDs.Count() > 0)
{
return_id = m_FreeEffectIDs.Get(0);
m_FreeEffectIDs.Remove(0);
}
else
{
return_id = m_HighestFreeEffectID;
++m_HighestFreeEffectID;
}
return return_id;
}
static bool DestroySound(EffectSound sound_effect)
{
DestroyEffect(sound_effect);
return true;
}
static SoundParams GetCachedSoundParam(string soundset)
{
SoundParams params;
if (!m_ParamsMap.Find(soundset, params))
{
params = new SoundParams(soundset);
m_ParamsMap.Insert(soundset, params);
}
return params;
}
static void Event_OnSoundWaveEnded(EffectSound effect_sound)
{
}
static void Event_OnFrameUpdate(float time_delta)
{
Event_OnFrameUpdate.Invoke(time_delta);
}
static void Init()
{
m_EffectsMap = new map<int, ref Effect>;
m_FreeEffectIDs = new array<int>;
m_ParamsMap = new map<string, ref SoundParams>;
Event_OnFrameUpdate = new ScriptInvoker();
m_IsInitialized = true;
}
static void InitServer()
{
m_EffectersMap = new map<int, EffecterBase>;
m_FreeEffecterIDs = new array<int>;
}
static void Cleanup()
{
// Nothing to clean
if (!m_IsInitialized)
return;
m_IsCleanup = true;
// There should not be anything in here on server
if (g_Game && g_Game.IsDedicatedServer())
{
if (m_ParamsMap.Count() > 0)
ErrorEx(string.Format("SEffectManager containing SoundParams on server."), ErrorExSeverity.WARNING);
if (m_EffectsMap.Count() > 0)
ErrorEx(string.Format("SEffectManager containing Effect on server."), ErrorExSeverity.WARNING);
}
// These are intentionally cached, just clear them
m_ParamsMap.Clear();
// These might not be intentionally still here, so log how many there are
#ifdef DEVELOPER
Print("--- SEffectManager Cleanup dump - Begin ------------------------");
Print(string.Format("Effect count: %1", m_EffectsMap.Count()));
#endif
// Best to call the unregister event before clearing the map
// In case some ref is still being held elsewhere and will still be kept alive
foreach (int id, Effect eff : m_EffectsMap)
{
eff.Event_OnUnregistered();
#ifdef SFXM_DUMP
Print(string.Format( "%1 :: %2 :: %3", eff, typename.EnumToString(EffectType, eff.GetEffectType()), eff.GetDebugName() ));
#endif
}
foreach (int i, EffecterBase effecter : m_EffectersMap)
{
effecter.Delete();
}
#ifdef DEVELOPER
Print("--- SEffectManager Cleanup dump - End --------------------------");
#endif
// Now we can clear it
m_EffectsMap.Clear();
m_EffectersMap.Clear();
// Reset the state
m_HighestFreeEffectID = 1;
Event_OnFrameUpdate.Clear();
m_IsCleanup = false;
}
static int CreateParticleServer(vector pos, EffecterParameters parameters)
{
EffecterBase eff;
eff = EffecterBase.Cast(g_Game.CreateObjectEx(parameters.m_EffecterType, pos, ECE_PLACE_ON_SURFACE));
if (eff)
{
int id = GetFreeEffecterID();
m_EffectersMap.Insert(id, eff);
}
eff.Init(id, parameters);
return id;
}
static void ReinitParticleServer(int effecterID, EffecterParameters parameters)
{
EffecterBase eff = m_EffectersMap.Get(effecterID);
if (eff)
{
eff.Init(effecterID,parameters);
}
}
static void ReactivateParticleServer(int effecterID)
{
EffecterBase eff = m_EffectersMap.Get(effecterID);
if (eff)
{
eff.Reactivate();
}
}
static void StartParticleServer(int effecterID)
{
EffecterBase eff = m_EffectersMap.Get(effecterID);
if (eff)
{
eff.Start();
}
}
static void StopParticleServer(int effecterID)
{
EffecterBase eff = m_EffectersMap.Get(effecterID);
if (eff)
{
eff.Stop();
}
}
static void DestroyEffecterParticleServer(int effecterID)
{
EffecterBase eff = m_EffectersMap.Get(effecterID);
if (eff)
{
m_EffectersMap.Remove(effecterID);
eff.DeleteSafe();
}
}
static void OnUpdate(float timeslice)
{
if (m_EffectersMap)
{
foreach (int i, EffecterBase effecter : m_EffectersMap)
{
effecter.DecreaseLifespan(timeslice);
}
}
}
}
enum EffecterCommands
{
NONE = -1,
START,
STOP,
REACTIVATE0,
REACTIVATE1
}
class EffecterParameters
{
string m_EffecterType;
float m_Lifespan;
void EffecterParameters(string type, float lifespan)
{
m_EffecterType = type;
m_Lifespan = lifespan;
}
}
class ParticleEffecterParameters : EffecterParameters
{
int m_ParticleID;
void ParticleEffecterParameters(string type, float lifespan, int particleID)
{
m_ParticleID = particleID;
}
}
class EffecterBase : EntityAI
{
const float NOT_DEFINED_LIFESPAN = -1;
protected float m_Lifespan;
protected int m_ID;
protected int m_Command = EffecterCommands.NONE;
protected int m_CommandSync = EffecterCommands.NONE;
void EffecterBase()
{
RegisterNetSyncVariableInt("m_CommandSync");
}
void Init(int id, EffecterParameters parameters)
{
m_ID = id;
SetLifespan(parameters.m_Lifespan);
}
void DecreaseLifespan(float timeSlice)
{
if (m_Lifespan == NOT_DEFINED_LIFESPAN)
return;
m_Lifespan -= timeSlice;
if (m_Lifespan < 0)
{
SEffectManager.DestroyEffecterParticleServer(m_ID);
}
}
void SetLifespan(float lifespan)
{
m_Lifespan = lifespan;
}
void Start()
{
m_CommandSync = EffecterCommands.START;
Process();
}
void Stop()
{
m_CommandSync = EffecterCommands.STOP;
Process();
}
void Reactivate()
{
if (m_CommandSync == EffecterCommands.REACTIVATE0)
{
m_CommandSync = EffecterCommands.REACTIVATE1;
}
else
{
m_CommandSync = EffecterCommands.REACTIVATE0;
}
Process();
}
void Process()
{
if (g_Game.IsMultiplayer())
{
SetSynchDirty();
}
else
{
OnVariablesSynchronized();
}
}
override int GetHideIconMask()
{
return EInventoryIconVisibility.HIDE_VICINITY;
}
}
class ParticleEffecter : EffecterBase
{
protected int m_ParticleEffectID = -1;
protected int m_ParticleEffectIDSync = -1;
protected ref EffectParticleGeneral m_Effect = null;
//protected int m_EffectID = -1;
void ParticleEffecter(int lifespan)
{
RegisterNetSyncVariableInt("m_ParticleEffectIDSync");
}
override void Init(int id, EffecterParameters parameters)
{
super.Init(id, parameters);
ParticleEffecterParameters par = ParticleEffecterParameters.Cast(parameters);
SetParticle(par.m_ParticleID);
}
void SetParticle(int particleID)
{
m_ParticleEffectIDSync = particleID;
Process();
}
override void OnVariablesSynchronized()
{
if (m_ParticleEffectIDSync != m_ParticleEffectID)
{
if (m_Effect)
{
m_Effect.SetParticle(m_ParticleEffectIDSync);
}
else
{
m_Effect = new EffectParticleGeneral();
m_Effect.SetParticle(m_ParticleEffectIDSync);
SEffectManager.PlayInWorld(m_Effect, GetWorldPosition());
}
m_ParticleEffectID = m_ParticleEffectIDSync;
}
if (m_CommandSync != m_Command)
{
switch (m_CommandSync)
{
case EffecterCommands.START:
if (m_Effect && !m_Effect.IsPlaying())
{
m_Effect.SetParticle(m_ParticleEffectID);
m_Effect.Start();
}
break;
case EffecterCommands.STOP:
if (m_Effect && m_Effect.IsPlaying())
{
m_Effect.Stop();
}
break;
case EffecterCommands.REACTIVATE0:
case EffecterCommands.REACTIVATE1:
if (m_Effect)
{
m_Effect.SetParticle(m_ParticleEffectID);
}
if (!m_Effect.IsPlaying())
{
m_Effect.Start();
}
break;
default:
break;
}
m_Command = m_CommandSync;
}
}
void ~ParticleEffecter()
{
SEffectManager.DestroyEffect(m_Effect);
}
}
class EffectParticleGeneral : EffectParticle
{
int m_LastParticleID;
void EffectParticleGeneral()
{
}
void SetParticle( int particleID )
{
bool was_playing = IsPlaying();
Stop();
SetParticleID(particleID);
if (was_playing)
{
Start();
}
}
override void SetParticleID( int id )
{
super.SetParticleID(id);
m_LastParticleID = id;
}
}