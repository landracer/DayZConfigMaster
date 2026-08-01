// ======================================================================
// File: effectsound_8c_source.html
// Category: other
// ======================================================================

class EffectSound : Effect
{
ref ScriptInvoker Event_OnSoundWaveStarted = new ScriptInvoker();
ref ScriptInvoker Event_OnSoundWaveEnded = new ScriptInvoker();
ref ScriptInvoker Event_OnSoundFadeInStopped = new ScriptInvoker();
ref ScriptInvoker Event_OnSoundFadeOutStarted = new ScriptInvoker();
protected ref SoundParams m_SoundParams;
protected ref SoundObjectBuilder m_SoundObjectBuilder;
protected ref SoundObject m_SoundObject;
protected AbstractWave m_SoundWaveObject;
protected WaveKind m_SoundWaveKind;
protected string m_SoundSetName;
protected bool m_SoundLoop;
protected bool m_SetEnvVariables;
protected bool m_SoundAutodestroy;
protected bool m_SoundWaveIsPlaying;
protected float m_SoundWaveLenght;
protected float m_SoundWaveVolume;
protected float m_SoundWaveVolumeMax;
protected float m_SoundWaveTime;
protected int m_SoundDoppler;
protected bool m_SoundWaveStarting;
protected bool m_SoundWaveStopping;
protected bool m_SoundFadedOut;
protected float m_SoundFadeInDuration;
protected float m_SoundFadeOutStartTime;
protected float m_SoundFadeOutDuration;
protected float m_SoundFadeOutInitVolume;
void EffectSound()
{
m_SoundWaveKind = WaveKind.WAVEEFFECTEX;
m_SoundWaveVolume = 0;
m_SoundWaveVolumeMax = 1;
m_SoundAutodestroy = false;
m_SoundWaveStopping = false;
m_SoundFadedOut = false;
m_SoundDoppler = -1;
}
void ~EffectSound()
{
}
override void InitEffect()
{
super.InitEffect();
// These will be called by the sound events
Event_OnStarted.Remove(Event_OnEffectStarted);
Event_OnStopped.Remove(Event_OnEffectEnded);
}
override string GetDebugName()
{
string identifier;
if (m_SoundSetName != "")
{
identifier = m_SoundSetName;
}
else
{
identifier = "NO_SOUNDSET";
}
return string.Format("%1:%2", super.GetDebugName(), identifier);
}
override EffectType GetEffectType()
{
return EffectType.SOUND;
}
override bool IsSound()
{
return true;
}
bool SoundPlayEx(out SoundParams params)
{
super.Start();
if (m_SoundSetName != "")
{
vector position = GetCurrentLocalPosition();
if ( SoundLoadEx(params) )
{
if (m_SetEnvVariables && m_SoundParams)
{
m_SoundObjectBuilder.AddEnvSoundVariables(GetPosition());
m_SoundObject = m_SoundObjectBuilder.BuildSoundObject();
m_SoundObject.SetKind( m_SoundWaveKind );
m_SoundObject.SetParent( m_ParentObject, m_PivotIndex );
}
if ( m_SoundObject )
{
SetCurrentLocalPosition(position, false);
m_SoundWaveObject = g_Game.GetSoundScene().Play3D( m_SoundObject, m_SoundObjectBuilder );
if ( !m_SoundWaveObject )
return false;
// Wait for header to be loaded before asking for its length, else we block the main thread
if (m_SoundWaveObject.IsHeaderLoaded())
ValidateSoundWave();
else
m_SoundWaveObject.GetEvents().Event_OnSoundWaveHeaderLoaded.Insert(ValidateSoundWave);
return true;
}
else
{
SoundError("m_SoundObject is null.");
}
}
}
return false;
}
bool SoundPlay()
{
SoundParams params;
return SoundPlayEx(params);
}
override void Start()
{
SoundPlay();
}
void SoundStop()
{
super.Stop();
if ( IsSoundPlaying() )
{
if ( m_SoundFadeOutDuration > 0 && !m_SoundWaveStopping )
{
m_SoundWaveStopping = true;
m_SoundFadedOut = false;
m_SoundWaveStarting = false;
m_SoundFadeOutStartTime = m_SoundWaveTime;
}
else
{
m_SoundWaveObject.Stop();
}
}
else if (!IsPendingDeletion()) // not about to be destroyed
{
SoundReset();
}
}
override void Stop()
{
SoundStop();
}
protected void SoundReset()
{
m_IsPlaying = false;
m_SoundWaveIsPlaying = false;
m_SoundWaveStopping = false;
m_SoundFadedOut = false;
m_SoundWaveVolume = m_SoundWaveVolumeMax;
m_SoundWaveTime = 0;
m_SoundFadeOutInitVolume = 0;
m_SoundFadeOutStartTime = 0;
if ( m_SoundWaveObject )
{
m_SoundWaveObject.Stop();
m_SoundWaveObject.SetVolumeRelative( m_SoundWaveVolumeMax );
}
}
bool IsSoundPlaying()
{
return m_SoundWaveIsPlaying;
}
override bool IsPlaying()
{
return IsSoundPlaying(); // Just in case, as that&#39;s what used to be the actual way to check
}
bool SoundLoadEx(out SoundParams params)
{
if ( !m_SoundParams || !m_SoundParams.IsValid() )
{
if (!params)
{
params = new SoundParams( m_SoundSetName );
}
//Print("SoundLoad is loading..");
m_SoundParams = params;
if ( !m_SoundParams.IsValid() )
{
SoundError("Invalid sound set.");
return false;
}
m_SoundObjectBuilder = new SoundObjectBuilder( m_SoundParams );
if (m_SetEnvVariables)
{
m_SoundObjectBuilder.AddEnvSoundVariables(GetPosition());
}
m_SoundObject = m_SoundObjectBuilder.BuildSoundObject();
if ( m_SoundObject )
{
m_SoundObject.SetKind( m_SoundWaveKind );
m_SoundObject.SetParent( m_ParentObject, m_PivotIndex );
}
else
{
SoundError("m_SoundObject is null.");
}
}
else
{
//Print("SoundLoad is loaded.");
}
return true;
}
bool SoundLoad()
{
SoundParams params;
return SoundLoadEx(params);
}
bool IsSoundValid()
{
return m_SoundParams.IsValid();
}
protected void ValidateSoundWave()
{
if (!m_SoundWaveObject)
{
ErrorEx(string.Format("%1 SoundWaveObject is null. SoundSet: %2", this.ToString(), m_SoundSetName));
return;
}
m_SoundWaveLenght = m_SoundWaveObject.GetLength();
if ( SoundWaveValidation() )
{
if ( m_SoundFadeInDuration > 0 )
{
m_SoundWaveObject.SetVolumeRelative( 0 );
m_SoundFadeOutStartTime = m_SoundWaveLenght - m_SoundFadeInDuration;
}
SetSoundLoop( m_SoundLoop );
m_SoundWaveStarting = true;
AbstractWaveEvents events = m_SoundWaveObject.GetEvents();
events.Event_OnSoundWaveStarted.Insert( Event_OnSoundWaveStarted );
events.Event_OnSoundWaveEnded.Insert( Event_OnSoundWaveEnded );
UpdateEvents();
}
else
{
m_SoundWaveObject.Stop();
}
}
protected bool SoundWaveValidation()
{
bool valid = true;
if ( m_SoundFadeInDuration > GetSoundWaveLength() )
{
SoundError("FadeIn is longer than sound wave length.");
valid = false;
}
if ( m_SoundFadeOutDuration > GetSoundWaveLength() )
{
SoundError("FadeOut is longer than sound wave length.");
valid = false;
}
if ( m_SoundFadeOutDuration + m_SoundFadeInDuration > GetSoundWaveLength() )
{
SoundError("FadeIn & FadeOut are longer than sound wave length.");
valid = false;
}
return valid;
}
protected void UpdateEvents()
{
if ( m_SoundWaveObject )
{
SetEnableEventFrame(true);
}
else
{
SetEnableEventFrame(false);
}
}
override void Event_OnFrameUpdate(float time_delta)
{
if ( IsSoundPlaying() )
{
if (m_SoundDoppler != -1)
{
m_SoundWaveObject.SetDoppler(m_SoundDoppler);
}
// FadeIn
if ( m_SoundWaveStarting )
{
if ( m_SoundFadeInDuration > 0 )
{
SetSoundVolume( GetSoundVolume() + (time_delta / m_SoundFadeInDuration) );
if ( GetSoundVolume() >= m_SoundWaveVolumeMax )
{
Event_OnSoundFadeInStopped();
SetSoundVolume( m_SoundWaveVolumeMax );
m_SoundWaveStarting = false;
}
}
else
{
SetSoundVolume( m_SoundWaveVolumeMax );
m_SoundWaveStarting = false;
}
}
// FadeOut
if ( m_SoundWaveStopping )
{
if ( m_SoundFadeOutDuration > 0 )
{
if ( m_SoundFadeOutInitVolume == 0 )
{
m_SoundFadeOutInitVolume = GetSoundVolume();
Event_OnSoundFadeOutStarted();
}
SetSoundVolume( GetSoundVolume() - (m_SoundFadeOutInitVolume / m_SoundFadeOutDuration * time_delta) );
}
else
{
SetSoundVolume( 0 );
}
if ( GetSoundVolume() <= 0 )
{
if ( m_SoundWaveObject )
{
m_SoundWaveObject.Stop();
m_SoundWaveStopping = false;
m_SoundFadedOut = true;
}
}
}
// Counting timer here because loop play
m_SoundWaveTime += time_delta;
}
}
override void Event_OnRegistered(int id)
{
super.Event_OnRegistered(id);
Event_OnSoundWaveEnded.Insert( SEffectManager.Event_OnSoundWaveEnded );
}
override void Event_OnUnregistered()
{
super.Event_OnUnregistered();
Event_OnSoundWaveEnded.Remove( SEffectManager.Event_OnSoundWaveEnded );
}
void Event_OnSoundWaveStarted()
{
m_SoundWaveIsPlaying = true;
Event_OnSoundWaveStarted.Invoke(this);
Event_OnEffectStarted();
}
void Event_OnSoundWaveEnded()
{
m_SoundWaveIsPlaying = false;
Event_OnSoundWaveEnded.Invoke(this);
Event_OnEffectEnded();
}
void Event_OnSoundFadeInStopped()
{
Event_OnSoundFadeInStopped.Invoke(this);
}
void Event_OnSoundFadeOutStarted()
{
Event_OnSoundFadeOutStarted.Invoke(this);
}
override void SetAutodestroy(bool auto_destroy)
{
super.SetAutodestroy(auto_destroy);
m_SoundAutodestroy = auto_destroy;
}
override bool IsAutodestroy()
{
return IsSoundAutodestroy();
}
void SetSoundAutodestroy(bool auto_destroy)
{
SetAutodestroy(auto_destroy);
}
bool IsSoundAutodestroy()
{
return m_SoundAutodestroy;
}
override bool CanDestroy()
{
return m_SoundFadeOutDuration <= 0 || !m_SoundWaveIsPlaying;
}
override void SetParent(Object parent_obj, int pivot)
{
super.SetParent(parent_obj, pivot); // ...
if (m_SoundObject)
{
m_SoundObject.SetParent(parent_obj, pivot);
}
}
override Object GetParent()
{
if (m_SoundObject)
return Object.Cast(m_SoundObject.GetParent());
else
return super.GetParent();
}
override int GetPivotIndex()
{
if (m_SoundObject)
return m_SoundObject.GetHierarchyPivot();
else
return super.GetPivotIndex();
}
override Object GetCurrentParent()
{
if (m_SoundObject)
return Object.Cast(m_SoundObject.GetParent());
else
return super.GetParent(); // Yes, intentionally this one
}
override void SetCurrentPosition( vector pos, bool updateCached = true )
{
super.SetCurrentPosition(pos, updateCached);
if (m_SoundObject)
{
Object parent = GetParent();
if (parent)
pos = parent.WorldToModel(pos);
m_SoundObject.SetPosition(pos);
}
}
override vector GetCurrentPosition()
{
if (m_SoundObject)
return m_SoundObject.GetPosition();
if (m_ParentObject)
return m_ParentObject.ModelToWorld(GetPosition());
return GetPosition();
}
override void SetCurrentLocalPosition( vector pos, bool updateCached = true )
{
super.SetCurrentLocalPosition(pos, updateCached);
if (m_SoundObject)
{
m_SoundObject.SetPosition(pos);
}
}
override vector GetCurrentLocalPosition()
{
Object parent = GetParent();
if (m_SoundObject)
{
//TODO(kumarjac): Create and expose &#39;SoundObject.GetLocalPosition&#39;
if (parent)
return parent.WorldToModel(m_SoundObject.GetPosition());
else
return m_SoundObject.GetPosition();
}
else
{
if (parent)
return GetLocalPosition();
else
return GetPosition();
}
return vector.Zero;
}
void SetSoundWaveKind(WaveKind wave_kind)
{
m_SoundWaveKind = wave_kind;
}
void SetSoundSet(string snd)
{
m_SoundSetName = snd;
}
string GetSoundSet()
{
return m_SoundSetName;
}
void SetSoundLoop(bool loop)
{
m_SoundLoop = loop;
if ( m_SoundWaveObject )
m_SoundWaveObject.Loop( loop );
}
void SetEnviromentVariables(bool setEnvVariables)
{
m_SetEnvVariables = setEnvVariables;
}
float GetSoundWaveLenght()
{
return GetSoundWaveLength();
}
float GetSoundWaveLength()
{
return m_SoundWaveLenght;
}
void SetSoundVolume(float volume)
{
m_SoundWaveVolume = volume;
if ( m_SoundWaveObject )
m_SoundWaveObject.SetVolumeRelative( volume );
}
float GetSoundVolume()
{
return m_SoundWaveVolume;
}
void SetSoundMaxVolume(float volume)
{
m_SoundWaveVolumeMax = volume;
if ( m_SoundWaveObject )
m_SoundWaveObject.SetVolumeRelative( m_SoundWaveVolume );
}
float GetSoundWaveTime()
{
return m_SoundWaveTime;
}
void SetSoundFadeIn(float fade_in)
{
m_SoundFadeInDuration = fade_in;
}
void SetSoundFadeOut(float fade_out)
{
m_SoundFadeOutDuration = fade_out;
}
void SetDoppler(bool setDoppler)
{
m_SoundDoppler = 0;
if (setDoppler)
{
m_SoundDoppler = 1;
}
}
protected void SoundError(string err_msg)
{
ErrorEx(string.Format("%1: SoundSetName: &#39;%2&#39; :: %3", this, m_SoundSetName, err_msg));
}
}