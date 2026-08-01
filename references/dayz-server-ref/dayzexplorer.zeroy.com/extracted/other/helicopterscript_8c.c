// ======================================================================
// File: helicopterscript_8c_source.html
// Category: other
// ======================================================================

class HelicopterScript extends HelicopterAuto
{
void HelicopterScript()
{
SetEventMask(EntityEvent.POSTSIMULATE);
}
override void EOnPostSimulate(IEntity other, float timeSlice)
{
}
bool OnBeforeEngineStart()
{
return true;
}
void OnEngineStart()
{
}
void OnEngineStop()
{
}
};