// ======================================================================
// File: effect_8c_source.html
// Category: other
// ======================================================================

enum EffectType
{
NONE,
SOUND,
PARTICLE,
}
class Effect : Managed
{
ref ScriptInvoker Event_OnStarted = new ScriptInvoker();
ref ScriptInvoker Event_OnStopped = new ScriptInvoker();
ref ScriptInvoker Event_OnEffectStarted = new ScriptInvoker();
ref ScriptInvoker Event_OnEffectEnded = new ScriptInvoker();
protected bool m_IsAutodestroy;
protected bool m_IsPendingDeletion;
protected bool m_IsPlaying;
protected Object m_ParentObject;
protected int m_PivotIndex = -1;
protected vector m_Position;
protected int m_ID;
protected bool m_IsRegistered;
protected vector m_LocalPos;
protected vector m_LocalOri;
void Effect()
{
if (g_Game.IsDedicatedServer())
{
ErrorEx("Created Effect on server.", ErrorExSeverity.WARNING);
}
m_IsPlaying = false;
InitEffect();
}
void ~Effect()
{
// Safety
if ( IsRegistered() )
SEffectManager.EffectUnregister(GetID());
// Certain effects need to be stopped to clean up properly
Stop();
// Another safety
SetEnableEventFrame(false);
}
void InitEffect()
{
Event_OnStarted.Insert(Event_OnEffectStarted);
Event_OnStarted.Insert(ValidateStart);
Event_OnStopped.Insert(Event_OnEffectEnded);
}
EffectType GetEffectType()
{
return EffectType.NONE;
}
bool IsSound()
{
return false;
}
bool IsParticle()
{
return false;
}
void Start()
{
// It is already playing!
if (IsPlaying())
return;
Event_OnStarted();
// I can&#39;t call this from inside the method with same name
// because that method is often overriden without super......
Event_OnStarted.Invoke(this);
}
void ValidateStart()
{
}
void Stop()
{
// It is not playing!
if (!IsPlaying())
return;
Event_OnStopped();
// Yes, that event is new, but let&#39;s keep up some consistency
Event_OnStopped.Invoke(this);
}
bool IsPlaying()
{
return m_IsPlaying;
}
protected void Destroy()
{
// Already queued
if (IsPendingDeletion())
return;
// Mark it to prevent queuing it up multiple times or get stuck in a call loop
m_IsPendingDeletion = true;
// Stop it, so that the effects can clean up themselves
// Since if for example this is EffectParticle and the particle is looping
// It NEEDS to be stopped to clean up the Particle
Stop();
// Queue up the destroying, as we should not do it while we are accessing it here
if (g_Game)
{
g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(SEffectManager.DestroyEffect, this);
}
}
void SetAutodestroy(bool auto_destroy)
{
m_IsAutodestroy = auto_destroy;
}
bool IsAutodestroy()
{
return m_IsAutodestroy;
}
bool IsPendingDeletion()
{
return m_IsPendingDeletion;
}
bool CanDestroy()
{
return true;
}
void SetEnableEventFrame(bool enable)
{
if ( enable )
{
SEffectManager.Event_OnFrameUpdate.Insert(Event_OnFrameUpdate);
}
else
{
SEffectManager.Event_OnFrameUpdate.Remove(Event_OnFrameUpdate);
}
}
void Event_OnStarted()
{
// Override this method for own use
}
void Event_OnStopped()
{
// Override this method for own use
}
void Event_OnEffectStarted()
{
m_IsPlaying = true;
Event_OnEffectStarted.Invoke(this);
}
void Event_OnEffectEnded()
{
m_IsPlaying = false;
Event_OnEffectEnded.Invoke(this);
if ( IsAutodestroy() )
{
Destroy();
}
}
void Event_OnFrameUpdate(float time_delta)
{
// Override this method for own use
}
void Event_OnRegistered(int id)
{
SetID(id);
m_IsRegistered = true;
}
void Event_OnUnregistered()
{
SetID(SEffectManager.INVALID_ID);
m_IsRegistered = false;
}
void OnCheckUpdate()
{
}
void SetParent(Object parent_obj, int pivot)
{
m_ParentObject = parent_obj;
m_PivotIndex = pivot;
}
void SetParent(Object parent_obj)
{
SetParent(parent_obj, -1);
}
Object GetParent()
{
return m_ParentObject;
}
int GetPivotIndex()
{
return m_PivotIndex;
}
void SetCurrentParent( Object parent_obj, bool updateCached = true )
{
if (updateCached)
SetParent(parent_obj);
}
Object GetCurrentParent()
{
return null;
}
void SetPosition( vector pos )
{
m_Position = pos;
}
vector GetPosition()
{
return m_Position;
}
void SetCurrentPosition( vector pos, bool updateCached = true )
{
if (updateCached)
SetPosition(pos);
}
vector GetCurrentPosition()
{
return vector.Zero;
}
void SetLocalPosition( vector pos )
{
m_LocalPos = pos;
}
vector GetLocalPosition()
{
return m_LocalPos;
}
void SetCurrentLocalPosition( vector pos, bool updateCached = true )
{
if (updateCached)
SetLocalPosition(pos);
}
vector GetCurrentLocalPosition()
{
return vector.Zero;
}
protected void SetID(int id)
{
m_ID = id;
}
int GetID()
{
return m_ID;
}
bool IsRegistered()
{
return m_IsRegistered;
}
void SetAttachmentParent(Object obj)
{
SetParent(obj);
}
Object GetAttachmentParent()
{
return GetParent();
}
void SetAttachedLocalPos(vector pos)
{
SetLocalPosition(pos);
}
vector GetAttachedLocalPos()
{
return GetLocalPosition();
}
void SetAttachedLocalOri(vector ori)
{
m_LocalOri = ori;
}
vector GetAttachedLocalOri()
{
return m_LocalOri;
}
}