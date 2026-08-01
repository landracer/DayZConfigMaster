// ======================================================================
// File: physics6dofjoint_8c_source.html
// Category: other
// ======================================================================

typedef int[] Physics6DOFJoint;
class Physics6DOFJoint: PhysicsJoint
{
proto external void SetLinearLimits(vector linearLower, vector linearUpper);
proto external void SetAngularLimits(vector angularLower, vector angularUpper);
proto external void SetLimit(int axis, float limitLower, float limitUpper);
}