// Source: F:/Games/Dayz/scripts/5_mission/dayzintroscenepc.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZIntroScenePC : DayZIntroScene
{
    protected bool      m_EnableClick;
    protected bool      m_RotatingCharacter;
    protected int       m_RotatingCharacterMouseX;
    protected int       m_RotatingCharacterMouseY;
    protected float     m_RotatingCharacterRot;
    protected float     m_Radius;
    protected float     m_Radius_original;
    protected float     m_DiffX;
    protected float     m_DeltaX;
    protected float     m_DeltaZ;
    protected float     m_Angle;
    protected float     m_Angle_offset = 0;
    protected float     m_NewX = 0;
    protected float     m_NewZ = 0;
    protected float     m_BlurValue;
 
    protected ref OptionsMenu m_optmenu;
 
    // ------------------------------------------------------------
    void DayZIntroScenePC()
    {
        SetClickEnable( true );
 
        m_DeltaX = Math.AbsFloat(m_CharacterPos[0] - m_Camera.GetPosition()[0]);
        m_DeltaZ = Math.AbsFloat(m_CharacterPos[2] - m_Camera.GetPosition()[2]);
        
        if (!m_Radius || m_Radius == 0)
        {
            m_Radius = Math.Sqrt (Math.Pow(m_DeltaX, 2) + Math.Pow(m_DeltaZ, 2));
            m_Radius_original = m_Radius;
        }
