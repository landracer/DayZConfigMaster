// ======================================================================
// File: particlebase_8c_source.html
// Category: other
// ======================================================================

class ParticleEvents
{
ref ScriptInvoker Event_OnParticleStart = new ScriptInvoker();
ref ScriptInvoker Event_OnParticleStop = new ScriptInvoker();
ref ScriptInvoker Event_OnParticleReset = new ScriptInvoker();
ref ScriptInvoker Event_OnParticleEnd = new ScriptInvoker();
ref ScriptInvoker Event_OnParticleParented = new ScriptInvoker();
ref ScriptInvoker Event_OnParticleUnParented = new ScriptInvoker();
}
class ParticleBase : Entity
{
protected bool m_IsPlaying;
protected ref ParticleEvents m_EventInvokers;
void ParticleBase()
{
m_EventInvokers = new ParticleEvents();
}
override bool IsParticle()
{
return true;
}
void PlayParticle(int particle_id = -1)
{
ErrorEx("Not implemented.", ErrorExSeverity.WARNING);
}
bool PlayParticleEx(int particle_id = -1, int flags = 0)
{
ErrorEx("Not implemented.", ErrorExSeverity.WARNING);
return false;
}
bool StopParticle(int flags = 0)
{
ErrorEx("Not implemented.", ErrorExSeverity.WARNING);
return false;
}
bool ResetParticle()
{
ErrorEx("Not implemented.", ErrorExSeverity.WARNING);
return false;
}
bool RestartParticle()
{
ErrorEx("Not implemented.", ErrorExSeverity.WARNING);
return false;
}
bool IsParticlePlaying()
{
ErrorEx("Not implemented.", ErrorExSeverity.WARNING);
return false;
}
ParticleEvents GetEvents()
{
return m_EventInvokers;
}
protected void OnParticleStart()
{
m_IsPlaying = true;
GetEvents().Event_OnParticleStart.Invoke(this);
}
protected void OnParticleStop()
{
m_IsPlaying = false;
GetEvents().Event_OnParticleStop.Invoke(this);
}
protected void OnParticleReset()
{
GetEvents().Event_OnParticleReset.Invoke(this);
}
protected void OnParticleEnd()
{
GetEvents().Event_OnParticleEnd.Invoke(this);
}
protected void OnParticleParented(IEntity parent)
{
GetEvents().Event_OnParticleParented.Invoke(this);
}
protected void OnParticleUnParented(IEntity parent)
{
GetEvents().Event_OnParticleUnParented.Invoke(this);
}
}