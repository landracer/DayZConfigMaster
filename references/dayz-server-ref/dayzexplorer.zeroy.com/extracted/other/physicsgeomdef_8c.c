// ======================================================================
// File: physicsgeomdef_8c_source.html
// Category: other
// ======================================================================

class PhysicsGeomDef: Managed
{
string Name;
dGeom Geometry;
vector Frame[4] = {Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Vector(0, 0, 0)};
int ParentNode = -1;
string MaterialName;
int LayerMask;
void PhysicsGeomDef(string name, dGeom geom, string materialName, int layerMask)
{
Name = name;
Geometry = geom;
MaterialName = materialName;
LayerMask = layerMask;
}
}