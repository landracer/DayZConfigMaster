// Source: F:/Games/Dayz/scripts/1_core/physics/physics6dofjoint.c Source File
// Extracted from DayZ Code Explorer by Zeroy



typedef int[] Physics6DOFJoint;
class Physics6DOFJoint: PhysicsJoint
{
    proto external void SetLinearLimits(vector linearLower, vector linearUpper);
    proto external void SetAngularLimits(vector angularLower, vector angularUpper);
    proto external void SetLimit(int axis, float limitLower, float limitUpper);
