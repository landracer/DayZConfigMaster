// Source: F:/Games/Dayz/scripts/1_core/proto/enphysics.c Source File
// Extracted from DayZ Code Explorer by Zeroy


 
typedef int[] dJoint;
typedef int[] dBlock;
 
proto native int dGetNumDynamicBodies(notnull IEntity worldEnt);
proto native IEntity dGetDynamicBody(notnull IEntity worldEnt, int index);
proto native void dSetInteractionLayer(notnull IEntity worldEntity, int mask1, int mask2, bool enable);
proto native bool dGetInteractionLayer(notnull IEntity worldEntity, int mask1, int mask2);

proto native vector dGetGravity(notnull IEntity worldEntity);
proto native void dSetGravity(notnull IEntity worldEntity, vector g);
proto native void dSetTimeSlice(notnull IEntity worldEntity, float timeSlice);

 
//proto native int dMaterialClone(string target, string source, int material_index)
//proto native int dMaterialGetType(string source)
//proto native int dMaterialSetType(string source, int material_index)

proto bool dBodyCreateStaticEx(notnull IEntity ent, PhysicsGeomDef geoms[]);
proto bool dBodyCreateGhostEx(notnull IEntity ent, PhysicsGeomDef geoms[]);

proto bool dBodyCreateDynamicEx(notnull IEntity ent, vector centerOfMass, float mass, PhysicsGeomDef geoms[]);

proto native void dBodyDestroy(notnull IEntity ent);

proto native bool dBodyIsSet(notnull IEntity ent);
 
proto native void dBodySetInteractionLayer(notnull IEntity ent, int mask);
proto native int dBodyGetInteractionLayer(notnull IEntity ent);
proto native void dBodySetGeomInteractionLayer(notnull IEntity ent, int index, int mask);
proto native int dBodyGetGeomInteractionLayer(notnull IEntity ent, int index);
    
proto native void dBodyActive(notnull IEntity ent, ActiveState activeState);
proto native void dBodyDynamic(notnull IEntity ent, bool dynamic);
proto native bool dBodyIsDynamic(notnull IEntity ent);
 
proto native bool dBodyIsActive(notnull IEntity ent);
proto native bool dBodyEnableGravity(notnull IEntity ent, bool enable);
proto native void dBodySetDamping(notnull IEntity ent, float linearDamping, float angularDamping);
proto native void dBodySetSleepingTreshold(notnull IEntity body, float linearTreshold, float angularTreshold);
 
proto native bool dBodyIsSolid(notnull IEntity ent);
proto native void dBodySetSolid(notnull IEntity ent, bool solid);

proto native void dBodyEnableCCD(notnull IEntity body, float maxMotion, float sphereCastRadius);
proto native void dBodySetLinearFactor(notnull IEntity body, vector linearFactor);

proto native vector dBodyGetCenterOfMass(notnull IEntity body);

proto native vector GetVelocity(notnull IEntity ent);

proto native void SetVelocity(notnull IEntity ent, vector vel);

proto native dBlock dBodyCollisionBlock(notnull IEntity ent1, notnull IEntity ent2);
proto native void dBodyRemoveBlock(notnull IEntity worldEntity, dBlock block);
 
proto native void dBodySetInertiaTensorV(notnull IEntity body, vector v);
proto native void dBodySetInertiaTensorM(notnull IEntity body, vector m[3]);
 
proto native float dBodyGetMass(notnull IEntity ent);
proto native void dBodySetMass(notnull IEntity body, float mass);
 
proto native void dBodyApplyTorqueImpulse(notnull IEntity ent, vector torqueImpulse);
proto native vector dBodyGetInvInertiaDiagLocal(notnull IEntity ent);
proto native float dBodyComputeImpulseDenominator(notnull IEntity ent, vector position, vector normal);
proto native float dBodyComputeAngularImpulseDenominator(notnull IEntity ent, vector axis);
proto native vector dBodyGetLocalInertia(notnull IEntity ent);
 
proto void dBodyGetInvInertiaTensorWorld(notnull IEntity body, out vector inertiaTensorWS[3]);

proto void dBodyApplyImpulseAt(notnull IEntity body, vector impulse, vector pos);

proto void dBodyApplyImpulse(notnull IEntity body, vector impulse);

proto void dBodyApplyForce(notnull IEntity body, vector force);

proto void dBodyApplyForceAt(notnull IEntity body, vector pos, vector force);
 
proto native void dBodyApplyTorque(notnull IEntity body, vector torque);

proto vector dBodyGetAngularVelocity(notnull IEntity body);

proto void dBodySetAngularVelocity(notnull IEntity body, vector angvel);

proto native void dBodySetTargetMatrix(notnull IEntity body, vector matrix[4], float timeslice);
 
proto native void dBodyGetWorldTransform(notnull IEntity body, out vector matrix[4]);
proto native void dBodyGetDirectWorldTransform(notnull IEntity body, out vector matrix[4]);
 
proto native float dBodyGetKineticEnergy(notnull IEntity body);
 
proto native vector dBodyGetVelocityAt(notnull IEntity body, vector globalpos);


proto native dGeom dGeomCreateBox(vector size);

proto native dGeom dGeomCreateSphere(float radius);

proto native dGeom dGeomCreateCapsule(float radius, vector extent);

proto native dGeom dGeomCreateCylinder(float radius, vector extent);

proto native void dGeomDestroy(dGeom geom);
 
//proto native int dBodyAddGeom(notnull IEntity body, dGeom geom, vector frame[4], string material, int interactionLayer);
// find a geometry by its name and returns its index or -1 if the geometry wasn&#39;t found
proto native int dBodyGetGeom(notnull IEntity ent, string name);
// returns number of geometries of the entity
proto native int dBodyGetNumGeoms(notnull IEntity ent);

 
proto native dJoint dJointCreateHinge(notnull IEntity ent1, notnull IEntity ent2, vector point1, vector axis1, vector point2, vector axis2, bool block, float breakThreshold);
proto native dJoint dJointCreateHinge2(notnull IEntity ent1, notnull IEntity ent2, vector matrix1[4], vector matrix2[4], bool block, float breakThreshold);
proto native dJoint dJointCreateSlider(notnull IEntity ent1, notnull IEntity ent2, vector matrix1[4], vector matrix2[4], bool block, float breakThreshold);
proto native dJoint dJointCreateBallSocket(notnull IEntity ent1, notnull IEntity ent2, vector point1, vector point2, bool block, float breakThreshold);
proto native dJoint dJointCreateFixed(notnull IEntity ent1, notnull IEntity ent2, vector point1, vector point2, bool block, float breakThreshold);
proto native dJoint dJointCreateConeTwist(notnull IEntity ent1, notnull IEntity ent2, vector matrix1[4], vector matrix2[4], bool block, float breakThreshold);
proto native dJoint dJointCreate6DOF(notnull IEntity ent1, notnull IEntity ent2, vector matrix1[4], vector matrix2[4], bool block, float breakThreshold);
proto native dJoint dJointCreate6DOFSpring(notnull IEntity ent1, notnull IEntity ent2, vector matrix1[4], vector matrix2[4], bool block, float breakThreshold);
proto native void dJointDestroy(dJoint joint);
 
//only hinge joint
proto native void dJointHingeSetLimits(dJoint joint, float low, float high, float softness, float biasFactor, float relaxationFactor);
proto native void dJointHingeSetAxis(dJoint joint, vector axis);
proto native void dJointHingeSetMotorTargetAngle(dJoint joint, float angle, float dt, float maxImpulse);
 
//only cone-twist joint
proto native void dJointConeTwistSetAngularOnly(dJoint joint, bool angularOnly);
// setLimit(), a few notes:
// _softness:
//      0->1, recommend ~0.8->1.
//      describes % of limits where movement is free.
//      beyond this softness %, the limit is gradually enforced until the &quot;hard&quot; (1.0) limit is reached.
// _biasFactor:
//      0->1?, recommend 0.3 +/-0.3 or so.
//      strength with which constraint resists zeroth order (angular, not angular velocity) limit violation.
// __relaxationFactor:
//      0->1, recommend to stay near 1.
//      the lower the value, the less the constraint will fight velocities which violate the angular limits.
proto native void dJointConeTwistSetLimit(dJoint joint, int limitIndex, float limitValue);
proto native void dJointConeTwistSetLimits(dJoint joint, float _swingSpan1, float _swingSpan2, float _twistSpan, float _softness, float _biasFactor, float _relaxationFactor);
 
//only 6DOF & 6DOFSpring.
proto native void   dJoint6DOFSetLinearLimits(dJoint joint, vector linearLower, vector linearUpper);
proto native void   dJoint6DOFSetAngularLimits(dJoint joint, vector angularLower, vector angularUpper);
proto native void dJoint6DOFSetLimit(dJoint joint, int axis, float lo, float hi);
 
//when stiffness == -1 && damping == -1, spring is disabled
proto native void dJoint6DOFSpringSetSpring(dJoint joint, int axis, float stiffness, float damping);
 
//only slider joint
proto native void dJointSliderSetLinearLimits(dJoint joint, float lowerLimit, float upperLimit);
proto native void dJointSliderSetAngularLimits(dJoint joint, float lowerLimit, float upperLimit);
proto native void dJointSliderSetDirLinear(dJoint joint, float softness, float restitution, float damping);
proto native void dJointSliderSetDirAngular(dJoint joint, float softness, float restitution, float damping);
proto native void dJointSliderSetLimLinear(dJoint joint, float softness, float restitution, float damping);
proto native void dJointSliderSetLimAngular(dJoint joint, float softness, float restitution, float damping);
proto native void dJointSliderSetOrthoLinear(dJoint joint, float softness, float restitution, float damping);
proto native void dJointSliderSetOrthoAngular(dJoint joint, float softness, float restitution, float damping);
//if force == 0, motor is off
proto native void dJointSliderSetLinearMotor(dJoint joint, float velocity, float force);
proto native void dJointSliderSetAngularMotor(dJoint joint, float velocity, float force);
proto native float dJointSliderGetLinearPos(dJoint joint);
proto native float dJointSliderGetAngularPos(dJoint joint);
 
//-----------------------------------------------------------------
typedef int[] dMaterial;

