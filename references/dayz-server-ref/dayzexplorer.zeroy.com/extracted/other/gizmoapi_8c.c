// ======================================================================
// File: gizmoapi_8c_source.html
// Category: other
// ======================================================================

enum GizmoTransformMode
{
NONE,
MOVE,
MOVE_2D,
ROTATE,
SCALE,
};
enum GizmoSpaceMode
{
WORLD,
LOCAL,
CAMERA,
};
typedef int GizmoHandle;
class GizmoApi : Managed
{
proto GizmoHandle GetCount();
proto Class GetInstance(GizmoHandle index);
proto Managed GetTracker(GizmoHandle index);
proto GizmoHandle FindByTracker(Managed tracker);
proto void DeselectTracker(Managed tracker);
proto void Deselect(GizmoHandle index);
proto void DeselectAll();
proto void SelectObject(Object object);
proto void SelectPhysics(Physics physics);
proto void SelectUser(Managed instance);
proto void SetTransformMode(GizmoTransformMode mode);
proto GizmoTransformMode GetTransformMode();
proto void SetSpaceMode(GizmoSpaceMode mode);
proto GizmoSpaceMode GetSpaceMode();
};
proto void CreateGizmoApi();
proto void DestroyGizmoApi();
proto GizmoApi GetGizmoApi();