// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindiagmenu/weaponliftdiag.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifdef DIAG_DEVELOPER
#ifndef SERVER
//----------------------------------------------------------------------------------------
class WeaponLiftDiagData
{
    vector               m_Transform[4];
    
    vector               m_Start;
    vector               m_Direction;
    float                m_WeaponStartDist
    float                m_WeaponEndDist;
    float                m_EffectiveAttachmentLength;
    float                m_ObstructionDistanceCfg;
    float                m_ObstructionDistance;
    
    float                m_RayRadius;
    float                m_RayRadiusOverride;
    ref RaycastRVResult  m_RayResult;
    float                m_HitFraction;
    float                m_HitDistance;
    
    vector               m_LastPosition;
    
    float                m_InterpInTime;
    float                m_InterpInTimeLift;
    float                m_InterpOutTime;
    float                m_InterpCurrTime;
    float                m_InterpCurrValue;
    float                m_InterpTgtValue;
    float                m_InterpSmoothValue;
    
    float                m_AimUD;
    float                m_AimLR;
    float                m_AimUD11;
    float                m_AimLR11;
    
    void SetTransform(vector transform[4])
    {
        for (int i = 0; i < 4; ++i) m_Transform[i] = transform[i];
    }
    
    void SetAimAngles(float udAngle, float lrAngle, float udAngle11, float lrAngle11)
    {
        m_AimUD   = udAngle;
        m_AimLR   = lrAngle;
        m_AimUD11 = udAngle11;
        m_AimLR11 = lrAngle11;
    }
    
    void SetWeaponRayParams(vector rayOrigin, vector rayDirection, float weaponStartDist, float weaponEndDist, float effectiveAttachmentLength, float obstructionDistanceCfg, float obstructionDistance, float rayRadius)
    {
        m_Start                     = rayOrigin;
        m_Direction                 = rayDirection;
        m_WeaponStartDist           = weaponStartDist;
        m_WeaponEndDist             = weaponEndDist;
        m_EffectiveAttachmentLength = effectiveAttachmentLength;
        m_ObstructionDistanceCfg    = obstructionDistanceCfg;
        m_ObstructionDistance       = obstructionDistance;
        m_RayRadius                 = rayRadius;
    }
    
    void SetIntersectionParams(RaycastRVResult rayResult, float hitFraction = 0, float hitDistance = 0)
    {
        m_RayResult   = rayResult;
        m_HitFraction = hitFraction;
        m_HitDistance = hitDistance;
    }
    
    void SetLastPosition(vector lastPosition)
    {
        m_LastPosition = lastPosition;
    }
    
    void SetInterpolation(float inTime, float inTimeLift, float outTime, float currentTime, float smoothValue, float currentValue, float targetValue)
    {       
        m_InterpInTime      = inTime;
        m_InterpInTimeLift  = inTimeLift;
        m_InterpOutTime     = outTime;
        m_InterpCurrTime    = currentTime;
        m_InterpSmoothValue = smoothValue;
        m_InterpCurrValue   = currentValue;
        m_InterpTgtValue    = targetValue;
    }
    
    void Clear()
    {
        Math3D.MatrixIdentity4(m_Transform);
        SetIntersectionParams(null, 0, 0);
        SetWeaponRayParams(vector.Zero, vector.Zero, 0, 0, 0, 0, 0, 0);
        SetLastPosition(vector.Zero);
        SetInterpolation(0,0,0,0,0,0,0);
        SetAimAngles(0,0,0,0);
    }
}
 
//----------------------------------------------------------------------------------------
class WeaponLiftDiag
{
    private ref WeaponLiftDiagData m_Data = new WeaponLiftDiagData();
    
    private const int COLOR_START        = 0xFF2ECC71; // Green
    private const int COLOR_END          = 0xFF4B77BE; // Blue
    private const int COLOR_END_NOATTACH = 0xFFF7CA18; // Yellow
    private const int COLOR_OBSTRUCTION  = 0xFFFF00FF; // Pink
    private const int COLOR_HIT          = 0xFFFF0000; // Red
    private const int COLOR_GUIDE        = 0xC03A3A3A; // Semi-transparent gray
    private const int COLOR_WEAPON       = 0x1FFFFFFF; // Semi-transparent white
    private const int COLOR_LAST_POS     = 0x3AF7CA18; // Semi-transparent yellow
    
    private bool m_bDrawGizmos    = true;
    private bool m_bDrawTransform = false;
    private bool m_bSmallShapes   = false;
    private bool m_bZBuffer       = false;
    private bool m_bWireOnly      = true;
    private bool m_bForceRaised   = false;
    private int  m_iColorCrosshair= false;
    
    private int m_iCrosshairR = 255;
    private int m_iCrosshairG = 0;
    private int m_iCrosshairB = 0;
    private int m_iCrosshairA = 255;
    
    private float m_fCrosshairBlendPow = 1;
    
    //----------------------------------------------------------------------------------------
    WeaponLiftDiagData Data()
    {
        return m_Data;
    }
    
    //----------------------------------------------------------------------------------------
    void Reset()
    {
        m_Data.Clear();
    }
    
    //----------------------------------------------------------------------------------------
    void DrawDiag(int mode, float deltaTime)
    {
        const int MODE_BASIC    = 1;
        const int MODE_EXTENDED = 2;
        
        
        bool wasColoringCrosshair = m_iColorCrosshair;
        
        WeaponLiftDiagData data = m_Data;
        if (data)
        {
            DbgUI.Begin(&quot;Weapon Lift/Obstruction Diag&quot;);
            {
                int gray            = 0x80FFFFFF; // semi transparent white (gray)
                int darkObst        = gray&COLOR_OBSTRUCTION;
                int endNoAttachCol  = gray & COLOR_END_NOATTACH;
                DbgUI.ColoredText(COLOR_WHITE,        &quot;Weapon properties&quot;);
                DbgUI.ColoredText(COLOR_START,        &quot;  weaponStartDist:        &quot; + data.m_WeaponStartDist);
                DbgUI.ColoredText(COLOR_WHITE,        &quot;  weaponLength:           &quot; + (data.m_WeaponEndDist - data.m_WeaponStartDist));
                DbgUI.ColoredText(COLOR_END,          &quot;  weaponEndDist:          &quot; + data.m_WeaponEndDist);
                if (data.m_EffectiveAttachmentLength > 0)
                {
                    DbgUI.ColoredText(gray,           &quot;    attachmentLength:     &quot; + data.m_EffectiveAttachmentLength);
                    DbgUI.ColoredText(endNoAttachCol, &quot;    distNoAttachment:     &quot; + (data.m_WeaponEndDist-data.m_EffectiveAttachmentLength));
                }
                
                DbgUI.ColoredText(COLOR_OBSTRUCTION,  &quot;  obstructionDist(cfg):   &quot; + data.m_ObstructionDistanceCfg);
                if (DbgUI.FloatOverride(&quot;weaponObstruction&quot;, data.m_ObstructionDistance, 0.0, data.m_WeaponEndDist - data.m_EffectiveAttachmentLength))
                {
                    data.m_ObstructionDistance += data.m_EffectiveAttachmentLength; // value was overriden directly, take attachment into account
                }
                
                DbgUI.ColoredText(darkObst,           &quot;    obstructionDist(act): &quot; + data.m_ObstructionDistance);
                DbgUI.ColoredText(COLOR_HIT,          &quot;  hitDistance:            &quot; + data.m_HitDistance);
                DbgUI.PushID_Str(&quot;rayRadiusText&quot;);
                DbgUI.ColoredText(0xFFAAAAAA,         &quot;  rayRadius:              &quot; + data.m_RayRadius );
                DbgUI.SameSpot();
                DbgUI.Text(                           &quot;                                &quot;); // ugly, but prevents some layouting spasms
                DbgUI.PopID();
                float rayRadius = data.m_RayRadius;
                if (DbgUI.FloatOverride(&quot;rayRadiusOverride&quot;, rayRadius, 0.0, 0.200, 1000))
                {
                    data.m_RayRadiusOverride = rayRadius;
                }
                else
                {
                    data.m_RayRadiusOverride = rayRadius;
                }
                
                DbgUI.Spacer(10);
                DbgUI.ColoredText(COLOR_WHITE, &quot;Utility&quot;);
                DbgUI.Check(&quot;Force raised&quot;, m_bForceRaised);
                TStringArray crossMode = { &quot;Off&quot;, &quot;On Obstruction&quot;, &quot;On Lift&quot;, &quot;On Both (Blend)&quot; };
                DbgUI.Combo(&quot;Color crosshair&quot;, m_iColorCrosshair, crossMode);
                
                if (m_iColorCrosshair)
                {
                    DbgUI.SameLine();
                    float r = m_iCrosshairR;
                    float g = m_iCrosshairG;
                    float b = m_iCrosshairB;
                    float a = m_iCrosshairA;
                    DbgUI.ColoredText(ARGB(m_iCrosshairA, m_iCrosshairR, m_iCrosshairG, m_iCrosshairB), &quot; -|-&quot;);                    
                    //R
                    DbgUI.SliderFloat(&quot;Red:      &quot;, r, 0, 255);
                    DbgUI.SameLine();
                    DbgUI.PushID_Str(&quot;ObstructionDiagColorR&quot;);
                    DbgUI.ColoredText(0xFFFF0000, m_iCrosshairR.ToString());
                    DbgUI.PopID();
                    //G                 
                    DbgUI.SliderFloat(&quot;Green:    &quot;, g, 0, 255);
                    DbgUI.SameLine();
                    DbgUI.PushID_Str(&quot;ObstructionDiagColorG&quot;);
                    DbgUI.ColoredText(0xFF00FF00, m_iCrosshairG.ToString());
                    DbgUI.PopID();
                    //B
                    DbgUI.SliderFloat(&quot;Blue:     &quot;, b, 0, 255);
                    DbgUI.SameLine();
                    DbgUI.PushID_Str(&quot;ObstructionDiagColorB&quot;);
                    DbgUI.ColoredText(0xFF0000FF, m_iCrosshairB.ToString());
                    DbgUI.PopID();
                    // A
                    DbgUI.SliderFloat(&quot;Alpha:    &quot;, a, 0, 255);
                    DbgUI.SameLine();
                    DbgUI.PushID_Str(&quot;ObstructionDiagColorA&quot;);
                    DbgUI.ColoredText(0xAAFFFFFF, m_iCrosshairA.ToString());
                    DbgUI.PopID();
                    m_iCrosshairR = r;
                    m_iCrosshairG = g;
                    m_iCrosshairB = b;
                    m_iCrosshairA = a;
                    
                    float pow = m_fCrosshairBlendPow * 5000;
                    DbgUI.SliderFloat(&quot;BlendPow: &quot;, pow, 1, 5000);
                    DbgUI.SameLine();
                    pow = pow / 1000.0;
                    DbgUI.Text(pow.ToString());
                    m_fCrosshairBlendPow = pow;
                }
                
                
                if (mode > MODE_BASIC)
                {
                    DbgUI.Spacer(10);
                    DbgUI.ColoredText(COLOR_WHITE, &quot;Intersection data&quot;);
                    if (data.m_RayResult && data.m_RayResult.surface)
                    {
                        DbgUI.Text(&quot;  Name:          &quot; + data.m_RayResult.surface.GetName());
                        DbgUI.Text(&quot;  EntryName:     &quot; + data.m_RayResult.surface.GetEntryName());
                        DbgUI.Text(&quot;  SurfaceType:   &quot; + data.m_RayResult.surface.GetSurfaceType());
                        
                        DbgUI.Text(&quot;  IsPassThrough: &quot; + data.m_RayResult.surface.IsPassthrough());
                        DbgUI.Text(&quot;  IsSolid:       &quot; + data.m_RayResult.surface.IsSolid());
                        
                        DbgUI.Text(&quot;  HitFraction:   &quot; + data.m_HitFraction);
                    }
                    else
                    {
                        DbgUI.Text(&quot;  Name:           - &quot; );
                        DbgUI.Text(&quot;  EntryName:      - &quot; );
                        DbgUI.Text(&quot;  SurfaceType:    - &quot; );
                        
                        DbgUI.Text(&quot;  IsPassThrough:  - &quot; );
                        DbgUI.Text(&quot;  IsSolid:        - &quot; );
                        
                        DbgUI.Text(&quot;  HitFraction:    - &quot; );
                    }
                }
                
                if (mode > MODE_BASIC)
                {
                    DbgUI.Spacer(10);
                    DbgUI.ColoredText(COLOR_WHITE, &quot;Interpolation&quot;);
                    DbgUI.Text(&quot;  In time:      &quot; + data.m_InterpInTime);
                    DbgUI.Text(&quot;  In time lift: &quot; + data.m_InterpInTimeLift);
                    DbgUI.Text(&quot;  Out time:     &quot; + data.m_InterpOutTime);
                    DbgUI.Text(&quot;  Current time: &quot; + data.m_InterpCurrTime);
                    DbgUI.Text(&quot;  Actual value: &quot; + data.m_InterpCurrValue);
                    DbgUI.Text(&quot;  Target value: &quot; + data.m_InterpTgtValue);
                    DbgUI.Text(&quot;  Smooth value: &quot; + data.m_InterpSmoothValue);
                }
                
                if (mode > MODE_BASIC)
                {
                    DbgUI.Spacer(10);
                    DbgUI.ColoredText(COLOR_WHITE, &quot;Aiming&quot;);
                    DbgUI.ColoredText(0xFF80FF80, &quot;  UD Angle:   &quot; + data.m_AimUD);
                    DbgUI.ColoredText(0xFFAAFFAA, &quot;  UD[-1,1]:   &quot; + data.m_AimUD11);
                    DbgUI.ColoredText(0xFFFF8080, &quot;  LR Angle:   &quot; + data.m_AimLR);
                    DbgUI.ColoredText(0xFFFFAAAA, &quot;  LR[-1,1]:   &quot; + data.m_AimLR11);
                }
                
                if (mode > MODE_BASIC)
                {
                    DbgUI.Spacer(10);
                    DbgUI.ColoredText(COLOR_WHITE, &quot;Preferences&quot;);
                    DbgUI.Check(&quot;  Draw shapes&quot;, m_bDrawGizmos);
                    DbgUI.Check(&quot;  Small shapes&quot;, m_bSmallShapes);
                    DbgUI.Check(&quot;  Z-buffer&quot;, m_bZBuffer);
                    DbgUI.Check(&quot;  Draw approx transform&quot;, m_bDrawTransform);
                    DbgUI.Check(&quot;  Wire only&quot;, m_bWireOnly);
                }
                
                if (mode >= MODE_BASIC)
                {
                    DbgUI.ColoredText(COLOR_WHITE, &quot;CfgGameplay WeaponObstructionData&quot;);
                    DbgUI.Text(&quot;  Static:   &quot; + EnumTools.EnumToString(EWeaponObstructionMode, CfgGameplayHandler.GetWeaponObstructionModeStatic()));
                    DbgUI.Text(&quot;  Dynamic:  &quot; + EnumTools.EnumToString(EWeaponObstructionMode, CfgGameplayHandler.GetWeaponObstructionModeDynamic()));
                }
            }
            DbgUI.End();
            
            // Override input! Extremely helpful!!
            if (m_bForceRaised && g_Game.GetPlayer())
            {
                DayZPlayer ply = g_Game.GetPlayer();
                HumanInputController hic = ply.GetInputController();
                hic.OverrideRaise( HumanInputControllerOverrideType.ONE_FRAME, true );
            }
            
            float radius = rayRadius;
            if (m_bSmallShapes) radius = 0.01;
            
            // gizmos
            if (m_bDrawTransform) Shape.CreateMatrix(data.m_Transform, 1.0, 0.05);
            
            // &quot;Apply&quot; settings 
            ShapeFlags flags = ShapeFlags.ONCE | ShapeFlags.TRANSP;
            ShapeFlags flags2 = ShapeFlags.ONCE | ShapeFlags.TRANSP |  ShapeFlags.NOOUTLINE;
            if (!m_bZBuffer)
            {
                flags |= ShapeFlags.NOZBUFFER;
                flags2 |= ShapeFlags.NOZBUFFER;
            }
            if (m_bWireOnly)
            {
                flags |= ShapeFlags.WIREFRAME;
            }
            else
            {
                flags |= ShapeFlags.NOOUTLINE;
            }
            
            vector wepStart       = data.m_Start + (data.m_WeaponStartDist * data.m_Direction);
            vector wepEnd         = data.m_Start + (data.m_WeaponEndDist * data.m_Direction);
            vector wepEndNoAttach = data.m_Start + ((data.m_WeaponEndDist - data.m_EffectiveAttachmentLength) * data.m_Direction);
            
            if (m_bDrawGizmos)
            {
                // Draw &quot;guide&quot; line, start->end 
                Shape.Create(ShapeType.LINE, COLOR_GUIDE, flags2, data.m_Start, wepEnd);
                // Draw weapon points
                Shape.CreateSphere(COLOR_START, flags, wepStart, radius);
                Shape.CreateSphere(COLOR_END_NOATTACH, flags, wepEndNoAttach, radius);
                Shape.CreateSphere(COLOR_END, flags, wepEnd, radius);
                // Draw obstruction distance  point
                if (data.m_ObstructionDistance != 0)
                {
                    vector wepObstr = data.m_Start + (data.m_ObstructionDistance * data.m_Direction);
                    Shape.CreateSphere(COLOR_OBSTRUCTION, flags, wepObstr, radius);
                }
                // Draw last hit position (in ext. mode)
                if (mode >= MODE_EXTENDED && data.m_LastPosition != vector.Zero)
                {
                    Shape.Create(ShapeType.LINE, COLOR_LAST_POS, flags2, data.m_Start, data.m_LastPosition);
                }
                // Draw the weapon as it moves (a that slides along the &quot;guide&quot; line)
                {
                    float  cylinderLength = (wepEnd - wepStart).Length();
                    vector cylinderOffset = 0.5 * cylinderLength * data.m_Transform[2];
                    Shape  cylinder       = Shape.CreateCylinder(COLOR_WHITE, flags2, vector.Zero, 0.3 * radius, cylinderLength);
                    vector tm0[4], tm1[3];
                    Math3D.DirectionAndUpMatrix(data.m_Transform[2], data.m_Transform[1], tm0);
                    Math3D.YawPitchRollMatrix(&quot;90 0 90&quot;, tm1);
                    Math3D.MatrixMultiply3(tm0, tm1, tm0);
                    float  fraction = Math.Clamp(data.m_HitFraction, 0, 1);
                    if (!data.m_RayResult) fraction = 1.0;
                    vector center = (data.m_Start + (fraction * (wepEnd - data.m_Start))) - cylinderOffset;
                    tm0[3] = center;
                    cylinder.SetMatrix(tm0);
                }
                // Draw hit
                if (data.m_RayResult)
                {
                    Shape.CreateSphere(COLOR_HIT,  flags, data.m_RayResult.pos, 0.5 * radius);
                }
            }
            
            if (m_iColorCrosshair || wasColoringCrosshair)
            {
                int crossColor = 0xFFFFFFFF;
                if (m_iColorCrosshair) // only set if not &quot;restoring&quot;
                {
                    
                    if (m_iColorCrosshair == 1) // obstruction
                    {
                        float hitFraction = Data().m_HitFraction;
                        if (hitFraction != 0 && hitFraction < 1.0)
                        {
                            crossColor = ARGB(m_iCrosshairA, m_iCrosshairR, m_iCrosshairG, m_iCrosshairB);
                        }
                    }
                    else if (m_iColorCrosshair == 2) // lift only
                    {                       
                        float hitDist = Data().m_HitDistance;
                        float obstDist = Data().m_ObstructionDistance;
                        if (hitDist != 0 && hitDist < obstDist)
                        {
                            crossColor = ARGB(m_iCrosshairA, m_iCrosshairR, m_iCrosshairG, m_iCrosshairB);
                        }
                    }
                    else if (m_iColorCrosshair == 3) // magic
                    {
                        float t = 0.0;
                        if (Data().m_HitFraction != 0)
                        {
                            float fr = 1.0-Math.InverseLerp( Data().m_ObstructionDistance, Data().m_WeaponEndDist, Data().m_HitDistance );
                            t = Math.Clamp( fr, 0.0, 1.0 );
                            t = Math.Pow(t, m_fCrosshairBlendPow);
                        }
                        
                        int white=0xFF;
                        int bA = Math.Lerp(white, m_iCrosshairA, t);
                        int bR = Math.Lerp(white, m_iCrosshairR, t);
                        int bG = Math.Lerp(white, m_iCrosshairG, t);
                        int bB = Math.Lerp(white, m_iCrosshairB, t);
                        
                        crossColor = ARGB(bA, bR, bG, bB);
                    }
                }
                
                WorkspaceWidget ww = g_Game.GetWorkspace();
                if (ww)
                {
                    Widget cw = ww.FindAnyWidget(&quot;CrossHairs&quot;);
                    if (cw)
                    {
                        Widget child = cw.GetChildren();
                        while (child)
                        {
                            child.SetColor(crossColor);
                            child = child.GetSibling();
                        }
                    }
                }
            }
        }
    }   
}
#endif
#endif
