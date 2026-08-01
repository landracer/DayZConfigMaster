// ======================================================================
// File: physics6dofspringjoint_8c_source.html
// Category: other
// ======================================================================

typedef int[] Physics6DOFSpringJoint;
class Physics6DOFSpringJoint: Physics6DOFJoint
{
proto external void SetSpring(int axis, float stiffness, float damping);
}