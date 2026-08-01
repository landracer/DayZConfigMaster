// Source: F:/Games/Dayz/scripts/1_core/physics/physicssliderjoint.c Source File
// Extracted from DayZ Code Explorer by Zeroy



typedef int[] PhysicsSliderJoint;
class PhysicsSliderJoint: PhysicsJoint
{
    proto external void SetLinearLimits(float lowerLimit, float upperLimit);
    proto external float GetLinearPos();
    proto external float GetAngularPos();
    proto external void SetAngularLimits(float lowerLimit, float upperLimit);
    proto external void SetDirLinear(float softness, float restitution, float damping);
    proto external void SetDirAngular(float softness, float restitution, float damping);
    proto external void SetLimLinear(float softness, float restitution, float damping);
    proto external void SetLimAngular(float softness, float restitution, float damping);
    proto external void SetOrthoLinear(float softness, float restitution, float damping);
    proto external void SetOrthoAngular(float softness, float restitution, float damping);
    proto external void SetLinearMotor(float velocity, float force);
    proto external void SetAngularMotor(float velocity, float force);
