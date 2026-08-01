// Source: F:/Games/Dayz/scripts/3_game/entities/camera.c Source File
// Extracted from DayZ Code Explorer by Zeroy

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
