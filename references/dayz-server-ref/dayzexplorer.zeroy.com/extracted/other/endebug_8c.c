// ======================================================================
// File: endebug_8c_source.html
// Category: other
// ======================================================================

proto void DumpStack();
proto void DumpStackString(out string stack);
proto void DebugBreak(bool condition = true, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
void CompileBreak();
proto void DPrint(string var);
enum ErrorExSeverity
{
INFO,
WARNING,
ERROR,
}
proto void ErrorEx(string err, ErrorExSeverity severity = ErrorExSeverity.ERROR);
proto void ErrorExString(string err, out string str, ErrorExSeverity severity = ErrorExSeverity.ERROR);
proto native void Error2(string title, string err);
void Error(string err)
{
Error2("", err);
}
proto void Print(void var);
proto void PrintToRPT(void var);
proto void PrintFormat(string fmt, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
//------------------------------------------
enum ShapeType
{
BBOX, //< Just box
LINE, //< One or more lines
SPHERE, //< Sphere represented by triangle mesh
CYLINDER, //< Cylinder represented by triangle mesh
DIAMOND, //< Eight faced pyramid. Defined by bound-box, where middle part is equal to horizontal extents of box and top/bottom apogees lies on top/bottom side of box.
PYRAMID //< Four sided pyramid. Defined by bound-box, where base is equal to bottom side of box.
};
enum ShapeFlags
{
NOZBUFFER, //< Do not compare z-buffer when render
NOZWRITE, //< Do not update z-buffer when render
WIREFRAME, //< Render just wire-frame outline. No solid faces
TRANSP, //< Is translucent
DOUBLESIDE, //< Double-sided (do not cull back-faces)
ONCE, //< Rendered just once and then it&#39;s automatically destroyed. Do not keep pointer to these!!
NOOUTLINE, //< Render just solid faces. No wire-frame outline.
BACKFACE, //< Render just back faces
NOCULL, //< Do not cull shapes by view frustum
VISIBLE, //< Make it visible. Set by default
ADDITIVE //< Additive blending (works with ShapeFlags.TRANSP)
};
enum CollisionFlags
{
FIRSTCONTACT, //<In many cases only collided=true/false is enough
NEARESTCONTACT, //<We want only one, the nearest contact
ONLYSTATIC, //<Only static objects
ONLYDYNAMIC, //<Only dynamic objects
ONLYWATER, //<Only water components (legacy support for "walk on geometry")
ALLOBJECTS, //<Valid when CF_FIRST_CONTACT, we get first contact for each object
}
class Shape
{
proto private void ~Shape();
proto native void GetMatrix(out vector mat[4]);
proto native void SetMatrix(vector mat[4]);
proto native void SetDirection(vector direction);
proto native void SetPosition(vector position);
proto native void SetColor(int color);
proto native void SetFlags(ShapeFlags flags);
proto native void Destroy();
proto static native Shape Create(ShapeType type, int color, ShapeFlags flags, vector p1, vector p2);
proto static native Shape CreateLines(int color, ShapeFlags flags, vector p[], int num);
proto static native Shape CreateTris(int color, ShapeFlags flags, vector p[], int num);
proto static native Shape CreateSphere(int color, ShapeFlags flags, vector origin, float radius);
proto static native Shape CreateFrustum(float horizontalAngle, float verticalAngle, float length, int color, ShapeFlags flags);
proto static native Shape CreateCylinder(int color, ShapeFlags flags, vector origin, float radius, float length);
static Shape CreateArrow(vector from, vector to, float size, int color, ShapeFlags flags)
{
vector dir = to - from;
dir.Normalize();
vector dir1 = dir * size;
size = size * 0.5;
vector dir2 = dir.Perpend() * size;
vector pts[5];
pts[0] = from;
pts[1] = to;
pts[2] = to - dir1 - dir2;
pts[3] = to - dir1 + dir2;
pts[4] = to;
return CreateLines(color, flags, pts, 5);
}
static Shape CreateBridgeArrow(vector from, vector to, float size, int color, ShapeFlags flags)
{
vector dir = to - from;
dir.Normalize();
vector dir1 = Vector(0, 0, -size);
size = size * 0.5;
vector dir2 = dir.Perpend() * size;
vector pts[7];
pts[0] = from;
pts[1] = from + "0 0 1";
pts[2] = to + "0 0 1";
pts[3] = to;
pts[4] = to - dir1 - dir2;
pts[5] = to - dir1 + dir2;
pts[6] = to;
return CreateLines(color, flags, pts, 7);
}
static void CreateMatrix(vector mat[4], float axisLength = 0.05, float arrowSize = 0.0)
{
int flags = ShapeFlags.ONCE | ShapeFlags.NOZWRITE | ShapeFlags.DOUBLESIDE | ShapeFlags.TRANSP;
if (arrowSize <= 0)
{
Create(ShapeType.LINE, 0xFFFF0000, flags, mat[3], mat[3] + axisLength * mat[0]);
Create(ShapeType.LINE, 0xFF00FF00, flags, mat[3], mat[3] + axisLength * mat[1]);
Create(ShapeType.LINE, 0xFF0000FF, flags, mat[3], mat[3] + axisLength * mat[2]);
}
else
{
CreateArrow(mat[3], mat[3] + axisLength * mat[0], arrowSize, 0xFFFF0000, flags);
CreateArrow(mat[3], mat[3] + axisLength * mat[1], arrowSize, 0xFF00FF00, flags);
CreateArrow(mat[3], mat[3] + axisLength * mat[2], arrowSize, 0xFF0000FF, flags);
}
}
};
//------------------------------------------
class DiagMenu
{
static proto bool IsInitialized();
static proto void InitScriptDiags();
static proto void ClearScriptDiags();
static proto void RegisterMenu(int id, string name, int parent);
static proto void RegisterItem(int id, string shortcut, string name, int parent, string values, func callback = null);
static proto void RegisterBool(int id, string shortcut, string name, int parent, bool reverse = false, func callback = null);
static proto void RegisterRange(int id, string shortcut, string name, int parent, string valuenames, func callback = null);
static proto void Unregister(int id);
static proto bool IsRegistered(int id);
static proto bool BindCallback(int id, func callback);
static proto void UnbindCallback(int id);
static proto bool GetBool(int id, bool reverse = false);
static proto int GetValue(int id);
static proto void SetValue(int id, int value);
static proto float GetRangeValue(int id);
static proto void SetRangeValue(int id, float value);
static proto int GetEngineValue(int id);
static proto void SetEngineValue(int id, int value);
static proto float GetEngineRangeValue(int id);
static proto void SetEngineRangeValue(int id, float value);
static proto bool MenuExists(string name);
};