// ======================================================================
// File: effectparticle_8c_source.html
// Category: other
// ======================================================================

class EffectParticle : Effect
{
protected Particle m_ParticleObj;
protected int m_ParticleID;
protected vector m_Orientation;
protected bool m_ForceRotationRelativeToWorld;
protected vector m_ParticleOrientation;
protected Object m_Object;
void EffectParticle()
{
}
void ~EffectParticle()
{
}
override void InitEffect()
{
super.InitEffect();
// Would be neat, but since particles are often already playing
// BEFORE they are even registered as the particle for the Effect
// Better to just keep that one I guess..
// Event_OnStarted.Remove(Event_OnEffectStarted);
// Will be called by the particle events
Event_OnStopped.Remove(Event_OnEffectEnded);
}
override string GetDebugName()
{
string identifier;
if (GetParticle())
{
identifier = GetParticle().GetDebugNameNative();
}
else
{
identifier = "NO_PARTICLE";
}
return string.Format("%1:%2:%3", super.GetDebugName(), m_ParticleID, identifier);
}
override void ValidateStart()
{
if (!GetParticle())
{
//ErrorEx(string.Format("No Particle started playing, stopping EffectParticle: %1", GetDebugName()), ErrorExSeverity.WARNING);
Stop();
}
}
override EffectType GetEffectType()
{
return EffectType.PARTICLE;
}
override bool IsParticle()
{
return true;
}
void SetParticle(Particle p)
{
// Unregister the events on the old
if (m_ParticleObj)
{
ParticleEvents ope = m_ParticleObj.GetEvents();
ope.Event_OnParticleStart.Remove(Event_OnEffectStarted);
ope.Event_OnParticleStop.Remove(Event_OnEffectEnded);
}
// Assign the new main Particle
m_ParticleObj = p;
// Register the events on the new
if (m_ParticleObj)
{
ParticleEvents npe = m_ParticleObj.GetEvents();
npe.Event_OnParticleStart.Insert(Event_OnEffectStarted);
// We will use Stop instead of End, as old particles were destroyed when they stopped
// And this system kinda relies on that
npe.Event_OnParticleStop.Insert(Event_OnEffectEnded);
}
}
Particle GetParticle()
{
return m_ParticleObj;
}
override void Start()
{
if (m_ParticleID > 0)
{
vector pos = GetPosition();
vector ori = GetOrientation();
if (m_ParentObject)
{
pos = GetLocalPosition();
ori = GetAttachedLocalOri();
}
SetParticle(ParticleManager.GetInstance().CreateParticle(m_ParticleID, pos, true, GetParent(), ori, IsParticleRotationRelativeToWorld()));
}
super.Start();
}
override void Stop()
{
if ( GetParticle() )
{
GetParticle().Stop();
SetParticle(null);
}
super.Stop();
}
void AttachTo(Object obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_rotation_to_world = false)
{
// Update the cached variables...
SetParent(obj);
SetLocalPosition(local_pos);
SetAttachedLocalOri(local_ori);
ForceParticleRotationRelativeToWorld(force_rotation_to_world);
// Now attach it
AddAsChild(obj, local_pos, local_ori, force_rotation_to_world);
}
void ReAttach()
{
// Skip the updating, as we are going to reuse what was set before
AddAsChild( GetParent(), GetLocalPosition(), GetAttachedLocalOri(), IsParticleRotationRelativeToWorld());
}
protected void AddAsChild(Object obj, vector local_pos, vector local_ori, bool force_rotation_to_world)
{
Particle p = GetParticle();
if (p)
{
p.AddAsChild(obj, local_pos, local_ori, force_rotation_to_world);
}
}
void Event_OnPlayStart()
{
}
void Event_OnPlayStarted()
{
}
void SetParticleID( int id )
{
m_ParticleID = id;
}
int GetParticleID()
{
return m_ParticleID;
}
void SetCurrentParticleID( int id )
{
m_ParticleID = id;
Particle p = GetParticle();
if (p)
{
p.SetSource(id);
}
}
int GetCurrentParticleID()
{
Particle p = GetParticle();
if (p)
{
return p.GetParticleID();
}
else
{
return ParticleList.INVALID;
}
}
override void SetCurrentParent( Object parent_obj, bool updateCached = true )
{
super.SetCurrentParent(parent_obj, updateCached);
ReAttach();
}
override Object GetCurrentParent()
{
Particle p = GetParticle();
if (p)
return Object.Cast(p.GetParent());
else
return super.GetParent();
}
override void SetCurrentPosition( vector pos, bool updateCached = true )
{
super.SetCurrentPosition(pos, updateCached);
Particle p = GetParticle();
if (p)
p.SetPosition(pos);
}
override vector GetCurrentPosition()
{
Particle p = GetParticle();
if (p)
return p.GetPosition();
else
return super.GetPosition();
}
override void SetCurrentLocalPosition( vector pos, bool updateCached = true )
{
super.SetCurrentLocalPosition(pos, updateCached);
Particle p = GetParticle();
if (p)
{
Object parent = GetParent();
if (parent)
ReAttach();
else
p.SetPosition(pos);
}
}
override vector GetCurrentLocalPosition()
{
Particle p = GetParticle();
if (p)
{
Object parent = GetParent();
if (parent)
return parent.WorldToModel(p.GetPosition());
else
return p.GetPosition();
}
else
return super.GetLocalPosition();
}
void SetOrientation( vector ori )
{
m_Orientation = ori;
}
vector GetOrientation()
{
return m_Orientation;
}
void SetCurrentOrientation( vector ori, bool updateCached = true )
{
if ( updateCached)
SetOrientation(ori);
Particle p = GetParticle();
if (p)
p.SetOrientation(ori);
}
vector GetCurrentOrientation()
{
Particle p = GetParticle();
if (p)
return p.GetOrientation();
else
return vector.Zero;
}
void ForceParticleRotationRelativeToWorld(bool state)
{
m_ForceRotationRelativeToWorld = state;
}
bool IsParticleRotationRelativeToWorld()
{
Particle p = GetParticle();
if (p)
return p.IsHierarchyPositionOnly();
else
return m_ForceRotationRelativeToWorld;
}
bool IsParticleCurrentRotationRelativeToWorld()
{
Particle p = GetParticle();
if (p)
return p.IsHierarchyPositionOnly();
else
return false;
}
void CheckLifeSpan()
{
/*
if ( !m_ParticleObj )
{
delete this;
}
OnCheckUpdate();
*/
}
void SetDecalOwner(Object o)
{
m_Object = o;
}
}