// Source: F:/Games/Dayz/scripts/4_world/entities/manbase/dayzplayer/dayzplayercameraironsights.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class DayZPlayerCameraIronsights extends DayZPlayerCameraBase
{
    static const float  CONST_UD_MIN    = -85.0;        
    static const float  CONST_UD_MAX    = 85.0;         
    static const float  CONST_UD_MIN_BACK = -25.0;      
 
    static const float  CONST_LR_MIN    = -160.0;       
    static const float  CONST_LR_MAX    = 160.0;            
    
    protected   bool            m_isEntering    = false;
    protected   bool            m_opticsHasWeaponOverride = false;
    protected ref array<float>  temp_array; //DOF only
    
    protected PPERequester_CameraADS m_RequesterADS;

    float m_velocityYaw[1];
    float m_velocityPitch[1];
    float m_dynamicsStrength;
    float m_dynamicsSmoothTime;
    vector m_SightMisalignmentModifier;
    
    float m_movementTimeAcc;
    float m_movementAmplitudeX;
    float m_movementAmplitudeY;
    float m_movementFrequencyX;
    float m_movementFrequencyY;
    
 
    void    DayZPlayerCameraIronsights(DayZPlayer pPlayer, HumanInputController pInput)
    {
        if (!temp_array)
            temp_array = new array<float>;
        m_iBoneIndex = pPlayer.GetBoneIndexByName(&quot;RightHand_Dummy&quot;);
        
        if (m_iBoneIndex == -1)
        {
            Print(&quot;DayZPlayerCamera1stPerson: main bone not found&quot;);
        }
 
        m_dynamicsStrength = 2;
        m_dynamicsSmoothTime = 0.3;
        
        SetupSightEntities();
        GetCurrentSightInfo(m_OpticsCamPos, m_OpticsCamDir);
        m_isEntering = true;
        m_WeaponSwayModifier = 0.5;
        
        m_movementTimeAcc = 0;
        m_movementAmplitudeX = 1;
        m_movementAmplitudeY = 1;
        m_movementFrequencyX = 1;
        m_movementFrequencyY = 2;
        
        Class.CastTo(m_RequesterADS,PPERequester_CameraADS.Cast(PPERequesterBank.GetRequester(PPERequester_CameraADS)));
        
        Init();
