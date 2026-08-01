// ======================================================================
// File: isboxcollidinggeometryproxyclasses_8c_source.html
// Category: other
// ======================================================================

class BoxCollidingParams
{
proto native void SetParams(vector center, vector orientation, vector edgeLength, ObjIntersect primaryType, ObjIntersect secondaryType, bool fullComponentInfo);
}
class ComponentInfo
{
int component;
vector componentMin;
vector componentMax;
vector componentCenter;
float componentRadius;
}
class BoxCollidingResult
{
Object obj;
Object parent;
int hierLevel;
int geomLevel;
ref ComponentInfoArray componentInfo;
ref ProxyInfoArray proxyInfo;
}
typedef array<ref ComponentInfo> ComponentInfoArray;
typedef array<ref BoxCollidingResult> ProxyInfoArray;