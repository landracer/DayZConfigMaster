// Source: F:/Games/Dayz/scripts/1_core/physics/physicsconetwistjoint.c Source File
// Extracted from DayZ Code Explorer by Zeroy



typedef int[] PhysicsConeTwistJoint;
class PhysicsConeTwistJoint: PhysicsJoint
{
    proto external void SetLimit(int limitIndex, float limitValue);
    proto external void SetLimits(float swingSpan1, float swingSpan2, float twistSpan, float softness, float biasFactor, float relaxationFactor);
