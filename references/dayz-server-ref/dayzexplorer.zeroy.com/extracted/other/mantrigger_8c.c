// ======================================================================
// File: mantrigger_8c_source.html
// Category: other
// ======================================================================

class ManTrigger : Trigger
{
override void EOnInit(IEntity other, int extra)
{
SetExtents("-2 -2 -2", "2 2 2");
}
override protected bool CanAddObjectAsInsider(Object object)
{
return object.IsMan();
}
};