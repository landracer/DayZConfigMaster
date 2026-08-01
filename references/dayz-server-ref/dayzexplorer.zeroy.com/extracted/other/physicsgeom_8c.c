// ======================================================================
// File: physicsgeom_8c_source.html
// Category: other
// ======================================================================

typedef PhysicsGeom dGeom;
typedef int[] PhysicsGeom;
class PhysicsGeom
{
proto external void Destroy();
static proto PhysicsGeom CreateBox(vector size);
static proto PhysicsGeom CreateSphere(float radius);
static proto PhysicsGeom CreateCapsule(float radius, float height);
static proto PhysicsGeom CreateCylinder(float radius, float height);
static proto PhysicsGeom CreateTriMesh(vector vertices[], int indices[], int numVertices, int numIndices);
}