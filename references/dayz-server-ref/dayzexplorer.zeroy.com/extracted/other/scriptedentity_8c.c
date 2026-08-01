// ======================================================================
// File: scriptedentity_8c_source.html
// Category: other
// ======================================================================

enum TriggerShape
{
BOX,
SPHERE,
CYLINDER,
}
class ScriptedEntityType : EntityAIType
{
};
class ScriptedEntity : EntityAI
{
proto native void SetClippingInfo(vector mins, vector maxs, float radius);
proto native void SetCollisionBox(vector mins, vector maxs);
proto native void SetCollisionSphere(float radius);
proto native void SetCollisionCylinder(float radius, float height);
private proto native void SetCollisionCylinderTwoWayNative(float radius, float negativeHeight, float positiveHeight);
void SetCollisionCylinderTwoWay(float radius, float negativeHeight, float positiveHeight)
{
if (radius <=0)
{
ErrorEx("Radius has to be > 0");
return;
}
if (negativeHeight > positiveHeight)
{
ErrorEx("Negative height cannot be higher than positive height");
return;
}
SetCollisionCylinderTwoWayNative(radius, negativeHeight, positiveHeight);
}
proto native void SetTriggerShape(TriggerShape shape);
proto native TriggerShape GetTriggerShape();
override bool IsInventoryVisible()
{
return false;
}
}