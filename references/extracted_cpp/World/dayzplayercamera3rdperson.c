// Source: F:/Games/Dayz/scripts/4_world/entities/manbase/dayzplayer/dayzplayercamera3rdperson.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// *************************************************************************************
// ! DayZPlayerCamera3rdPerson - base for 3rd person cameras
// *************************************************************************************
class DayZPlayerCamera3rdPerson extends DayZPlayerCameraBase
{
    static const float  CONST_UD_MIN    = -85.0;        
    static const float  CONST_UD_MAX    = 85.0;         
 
    static const float  CONST_LR_MIN    = -160.0;       
    static const float  CONST_LR_MAX    = 160.0;        
    
    void    DayZPlayerCamera3rdPerson(DayZPlayer pPlayer, HumanInputController pInput)
    {
        if (pInput.Camera3rdIsRightShoulder())
        {
            m_fCameraLRShoulder     = 1.0;
        }
        else
        {
            m_fCameraLRShoulder     = -1.0;
        }
 
        m_fCameraLRShoulderVel[0]   = 0.0;
        m_CurrentCameraRoll         = 0.0;
        m_WeaponSwayModifier        = 1;
        m_fLeanDistance             = 0.3;
        m_fPredictCollisionRadius   = 0.5;
