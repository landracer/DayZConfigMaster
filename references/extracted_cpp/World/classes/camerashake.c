// Source: F:/Games/Dayz/scripts/4_world/classes/camerashake.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CameraShake
{
    const float MIN_PLAYER_DISTANCE = 40;
    
    float m_Radius;
    float m_RadiusDecaySpeed;
    float m_RandomAngle;
    float m_Time;
    float m_InitLR;
    float m_InitUD;
    bool m_ToDelete;
    float m_Smoothness;
    float m_StregthFactor;
    DayZPlayerImplement m_Player;
    
    void ~CameraShake()
    {
        if(m_Player)
            m_Player.GetAimingModel().SetCamShakeValues(0, 0);
 
