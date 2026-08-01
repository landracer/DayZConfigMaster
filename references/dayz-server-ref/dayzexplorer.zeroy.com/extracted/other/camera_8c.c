// ======================================================================
// File: camera_8c_source.html
// Category: other
// ======================================================================

class Camera extends Entity
{
static proto native Camera GetCurrentCamera();
static proto native float GetCurrentFOV();
static proto native void InterpolateTo(Camera targetCamera, float time, int type);
static proto native bool IsInterpolationComplete();
proto native void SetNearPlane(float nearPlane);
proto native float GetNearPlane();
proto native void SetActive(bool active);
proto native void EnableSmooth(bool enable);
proto native void StopInterpolation();
proto native bool IsActive();
proto native void SetFOV(float fov);
proto native void SetFocus(float distance, float blur);
proto native void LookAt(vector targetPos);
};
//-----------------------------------------------------------------------------
class FreeDebugCamera extends Camera
{
static proto native FreeDebugCamera GetInstance();
proto native bool IsPlayerMove();
proto native void SetFreezed(bool freezed);
proto native bool IsFreezed();
proto native Object GetCrosshairObject();
};