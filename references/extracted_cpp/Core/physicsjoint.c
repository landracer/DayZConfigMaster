// Source: F:/Games/Dayz/scripts/1_core/physics/physicsjoint.c Source File
// Extracted from DayZ Code Explorer by Zeroy



typedef int[] PhysicsJoint;
class PhysicsJoint
{
    static proto PhysicsHingeJoint CreateHinge(notnull IEntity ent1, IEntity ent2, vector point1, vector axis1, vector point2, vector axis2, bool disableCollisions, float breakThreshold = -1);
    static proto PhysicsHingeJoint CreateHinge2(notnull IEntity ent1, IEntity ent2, vector matrix1[4], vector matrix2[4], bool disableCollisions, float breakThreshold = -1);
    static proto PhysicsBallSocketJoint CreateBallSocket(notnull IEntity ent1, IEntity ent2, vector point1, vector point2, bool disableCollisions, float breakThreshold = -1);
    static proto PhysicsFixedJoint CreateFixed(notnull IEntity ent1, IEntity ent2, vector point1, vector point2, bool disableCollisions, float breakThreshold = -1);
    static proto PhysicsConeTwistJoint CreateConeTwist(notnull IEntity ent1, IEntity ent2, vector matrix1[4], vector matrix2[4], bool disableCollisions, float breakThreshold = -1);
    static proto PhysicsSliderJoint CreateSlider(notnull IEntity ent1, IEntity ent2, vector matrix1[4], vector matrix2[4], bool disableCollisions, float breakThreshold = -1);
    static proto Physics6DOFJoint Create6DOF(notnull IEntity ent1, IEntity ent2, vector matrix1[4], vector matrix2[4], bool disableCollisions, float breakThreshold = -1);
    static proto Physics6DOFSpringJoint Create6DOFSpring(notnull IEntity ent1, IEntity ent2, vector matrix1[4], vector matrix2[4], bool disableCollisions, float breakThreshold = -1);
    proto external void Destroy();
