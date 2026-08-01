// Source: F:/Games/Dayz/scripts/1_core/physics/physics.c Source File
// Extracted from DayZ Code Explorer by Zeroy



typedef int[] Physics;
class Physics
{
    static const float KMH2MS = 1 / 3.6; //<! Constant for converting km/h to m/s
    static const float MS2KMH = 3.6; //<! Constant for converting m/s to km/h
    static const float STANDARD_GRAVITY = 9.81;
    static const vector VGravity = &quot;0 -9.81 0&quot;;

    proto external void Destroy();
    proto external void SetInteractionLayer(int mask);
    proto external void GetWorldTransform(out vector mat[4]);
    proto external void GetDirectWorldTransform(out vector mat[4]);
    proto external void SetGeomInteractionLayer(int index, int mask);
    proto external int GetInteractionLayer();
    proto external bool HasInteractionEnabled(int mask);
    proto external int GetGeomInteractionLayer(int index);
    proto external void EnableCCD(float maxMotion, float sphereCastRadius);
    proto external void ChangeSimulationState(SimulationState simState);
    proto external SimulationState GetSimulationState();
    proto external vector GetCenterOfMass();
    proto external void SetActive(ActiveState act);
    proto external bool IsActive();
    proto external bool IsDynamic();
    proto external bool IsKinematic();
    proto external bool EnableGravity(bool enable);
    proto external void SetDamping(float linearDamping, float angularDamping);
    proto external float GetMass();
    proto external void SetMass(float mass);
    proto external void SetInertiaTensorV(vector v);
    proto external vector GetAngularVelocity();
    proto external vector GetVelocityAt(vector pos);
    proto external void SetSleepingTreshold(float linearTreshold, float angularTreshold);
    proto external void SetLinearFactor(vector linearFactor);
    proto external vector GetVelocity();
    proto external void SetVelocity(vector velocity);
    proto external void SetAngularVelocity(vector velocity);
    proto external void SetTargetMatrix(vector matrix[4], float timeslice);
    proto external void ApplyImpulse(vector impulse);
    proto external void ApplyImpulseAt(vector pos, vector impulse);
    proto external void ApplyForce(vector force);
    proto external void ApplyForceAt(vector pos, vector force);
    proto external void ApplyTorque(vector torque);
    proto external void ClearForces();
    proto external vector GetTotalForce();
    proto external vector GetTotalTorque();
    proto external void SetResponseIndex(int responseIndex);
    proto external int GetResponseIndex();
    proto external int AddGeom(string name, PhysicsGeom geom, vector frame[4], string material, int interactionLayer);
    proto external int GetGeom(string name);
    proto external int GetNumGeoms();
    proto external bool UpdateGeometries(int interactionLayerAnd = 0xffffffff, int interactionLayerOr = 0);
    proto external string GetGeomName(int index);
    proto external void GetGeomWorldTransform(int index, out vector mat[4]);
    proto external void GetGeomTransform(int index, out vector mat[4]);
    proto external void GetGeomDirectTransform(int index, out vector mat[4]);
    proto external vector GetGeomWorldPosition(int index);
    proto external vector GetGeomPosition(int index);
    proto external vector GetGeomDirectPosition(int index);
    proto external void GetGeomWorldBounds(int index, out vector min, out vector max);
    proto external void GetGeomDirectBounds(int index, out vector min, out vector max);
    proto external void GetGeomSurfaces(int index, notnull out array<SurfaceProperties> surfaces);
    proto external int GetNumBones();
    proto external Physics GetBone(int index);
    static proto Physics CreateStatic(notnull IEntity ent, int layerMask = 0xffffffff);
    static proto Physics CreateStaticEx(notnull IEntity ent, PhysicsGeomDef geoms[]);
    static proto Physics CreateDynamic(notnull IEntity ent, float mass, int layerMask = 0xffffffff);
    static proto Physics CreateDynamicEx(notnull IEntity ent, vector centerOfMass, float mass, PhysicsGeomDef geoms[]);
    static proto Physics CreateGhostEx(notnull IEntity ent, PhysicsGeomDef geoms[]);
