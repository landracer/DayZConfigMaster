// Source: F:/Games/Dayz/scripts/2_gamelib/entities/scriptcamera.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifdef GAME_TEMPLATE
 
[EditorAttribute(&quot;box&quot;, &quot;GameLib/Scripted&quot;, &quot;Script camera&quot;, &quot;-0.25 -0.25 -0.25&quot;, &quot;0.25 0.25 0.25&quot;, &quot;255 0 0 255&quot;)]
class ScriptCameraClass
{
 
}
 
ScriptCameraClass ScriptCameraSource;
 
class ScriptCamera: GenericEntity
{
    [Attribute(&quot;60&quot;, &quot;slider&quot;, &quot;Field of view&quot;, &quot;0 180 1&quot;)]
    float FOV;
    [Attribute(&quot;1&quot;, &quot;editbox&quot;, &quot;Near plane clip&quot;)]
    float NearPlane;
    [Attribute(&quot;4000&quot;, &quot;editbox&quot;, &quot;Far plane clip&quot;)]
    float FarPlane;
    
    [Attribute(&quot;1&quot;, &quot;combobox&quot;, &quot;Projection type&quot;, &quot;&quot;, ParamEnumArray.FromEnum(CameraType) )]
    int Type;
    [Attribute(&quot;5&quot;, &quot;slider&quot;, &quot;Camera speed&quot;, &quot;0 20 1&quot;)]
    float Speed;
    [Attribute(&quot;1&quot;, &quot;combobox&quot;, &quot;Free Fly&quot;, &quot;&quot;, ParamEnumArray.FromEnum(EBool) )]
    bool FreeFly;
    [Attribute(&quot;0&quot;, &quot;combobox&quot;, &quot;Invert vertical&quot;, &quot;&quot;, ParamEnumArray.FromEnum(EBool) )]
    bool Inverted;
    [Attribute(&quot;0&quot;, &quot;slider&quot;, &quot;Camera index&quot;, &quot;0 31 1&quot;)]
    int Index;
    float m_MouseSensitivity = 0.001; // should be somewhere else.
    float m_GamepadSensitivity = 0.2; // should be somewhere else.
    int m_GamepadFreeFly;
 
    // debug variables
    int m_DbgListSelection = 0;
    ref array<string> m_DbgOptions = {&quot;Perspective&quot;, &quot;Orthographic&quot;};
 
    void ScriptCamera(IEntitySource src, IEntity parent)
    {
        SetFlags(EntityFlags.ACTIVE, false);
        SetEventMask(EntityEvent.FRAME);
 
        SetCameraVerticalFOV(Index, FOV);
        SetCameraFarPlane(Index, FarPlane);
        SetCameraNearPlane(Index, NearPlane);
        SetCameraType(Index, Type);
        m_DbgListSelection = Type - 1;
        SetCamera(Index, GetOrigin(), GetYawPitchRoll());
 
        vector camMat[4];
        GetTransform(camMat);
        SetCameraEx(Index, camMat);
        m_GamepadFreeFly = FreeFly;
    }
 
    override protected void EOnFrame(IEntity other, float timeSlice) //EntityEvent.FRAME
    {
        g_Game.GetInputManager().ActivateContext(&quot;ScriptCameraContext&quot;);
        
        if (g_Game.GetInputManager().GetActionTriggered(&quot;CamFreeFly&quot;))
        {
            FreeFly = !FreeFly;
        }
        
        if (FreeFly)
        {
            FreeFly(timeSlice);
        }
        else
        {
            vector camMat[4]; // matrix can be set outside the class
            GetTransform(camMat);
            SetCameraEx(Index, camMat);
        }
        
        if (GameSettings.Debug)
        {
            DebugInfo();
        }
    }
 
    protected void FreeFly(float timeSlice)
    {
        vector camPosition = GetOrigin();
        vector angles = GetYawPitchRoll();
        vector camMat[4];
        GetTransform(camMat);   
        InputManager imanager = g_Game.GetInputManager();
        imanager.ActivateContext(&quot;ScriptCameraFreeFlyContext&quot;);
        
        // get input
        float turnX         = imanager.LocalValue(&quot;CamTurnRight&quot;) * 20.0 * timeSlice;
        float turnY         = imanager.LocalValue(&quot;CamTurnUp&quot;) * 20.0 * timeSlice;
        float turnZ         = imanager.LocalValue(&quot;CamRotate&quot;) * 20.0 * timeSlice;
        float moveForward   = imanager.LocalValue(&quot;CamForward&quot;);
        float moveRight     = imanager.LocalValue(&quot;CamRight&quot;);
        float moveAscend    = imanager.LocalValue(&quot;CamAscend&quot;);
        float speedDelta    = imanager.LocalValue(&quot;CamSpeedDelta&quot;) * timeSlice;
        bool speedBoostHigh     = imanager.GetActionTriggered(&quot;CamSpeedBoostHigh&quot;);
        bool speedBoostLow  = imanager.GetActionTriggered(&quot;CamSpeedBoostLow&quot;);
 
        Speed = Math.Clamp(Speed + speedDelta * Speed * 0.25, 0.1, 1000.0);
        
        float finalSpeed = Speed;
        if (speedBoostLow)
            finalSpeed *= 25;
        else if (speedBoostHigh)
            finalSpeed *= 5;
 
        // rotation
        angles[0] = turnX + angles[0];
        if (Inverted)
            angles[1] = turnY + angles[1];
        else
            angles[1] = -turnY + angles[1];
 
        angles[2] = turnZ + angles[2];
        
        // movement
        vector move = vector.Zero;
        vector forward = camMat[2];
        vector up = camMat[1];
        vector side = camMat[0];
        
        move += forward * moveForward;
        move += side    * moveRight;
        move += up      * moveAscend;
        
        // ------------     
        camPosition = (move * timeSlice * finalSpeed) + camPosition;
        
        Math3D.YawPitchRollMatrix(angles, camMat);
        camMat[3] = camPosition;
        SetTransform(camMat);
        SetCameraEx(Index, camMat);
    }
 
    protected void DebugInfo()
    {
        InputManager imanager = g_Game.GetInputManager();
        DbgUI.Begin(String(&quot;Camera #&quot; + Index.ToString()), 0, Index * 300);
 
        DbgUI.Text(String(&quot;Position : &quot; + GetOrigin().ToString()));
        DbgUI.Text(String(&quot;Orientation (Y, P, R): &quot; + GetYawPitchRoll().ToString()));
        DbgUI.Text(String(&quot;Speed : &quot; + Speed.ToString()));
        DbgUI.Text(String(&quot;Mouse sensitivity : &quot; + (2000 - (1 / m_MouseSensitivity)).ToString()));
        DbgUI.Check(&quot;Select Free fly&quot;, FreeFly);
        DbgUI.List(&quot;Camera type&quot;, m_DbgListSelection, m_DbgOptions);
        if (m_DbgListSelection + 1 != Type)
        {
            Type = m_DbgListSelection + 1;
            SetCameraType(Index, Type);
        }
        
        float sensitivity = 2000 - (1 / m_MouseSensitivity);
        DbgUI.SliderFloat(&quot;Mouse sensitivity&quot;, sensitivity, 1, 1999);
        m_MouseSensitivity = 1 / (2000 - sensitivity);
        
        DbgUI.Text(&quot;CamTurnRight: &quot; + imanager.LocalValue(&quot;CamTurnRight&quot;));
        DbgUI.Text(&quot;CamTurnUp: &quot; + imanager.LocalValue(&quot;CamTurnUp&quot;));
        DbgUI.Text(&quot;CamSpeedDelta: &quot; + imanager.LocalValue(&quot;CamSpeedDelta&quot;));
        DbgUI.Text(&quot;CamForward: &quot; + imanager.LocalValue(&quot;CamForward&quot;));
        DbgUI.Text(&quot;CamRight: &quot; +imanager.LocalValue(&quot;CamRight&quot;));
        DbgUI.Text(&quot;CamAscend: &quot; + imanager.LocalValue(&quot;CamAscend&quot;));
        DbgUI.Text(&quot;CamSpeedBoostHigh: &quot; + imanager.GetActionTriggered(&quot;CamSpeedBoostHigh&quot;));
        DbgUI.Text(&quot;CamSpeedBoostLow:&quot; + imanager.GetActionTriggered(&quot;CamSpeedBoostLow&quot;));      
        
        DbgUI.End();
    }
}
 
#endif
