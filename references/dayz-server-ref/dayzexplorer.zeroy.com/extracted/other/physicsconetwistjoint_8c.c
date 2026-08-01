// ======================================================================
// File: physicsconetwistjoint_8c_source.html
// Category: other
// ======================================================================

typedef int[] PhysicsConeTwistJoint;
class PhysicsConeTwistJoint: PhysicsJoint
{
proto external void SetLimit(int limitIndex, float limitValue);
proto external void SetLimits(float swingSpan1, float swingSpan2, float twistSpan, float softness, float biasFactor, float relaxationFactor);
}