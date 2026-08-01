// Source: F:/Games/Dayz/scripts/4_world/classes/debugmonitorvalues.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DebugMonitorValues
{
    const int TYPE_HEALTH       = 1;
    const int TYPE_BLOOD        = 2;
    const int TYPE_BODY_TEMP    = 3;
    const int TYPE_POSITION     = 4;
    const int TYPE_LAST_DAMAGE  = 5;
    
    const int LAST_DAMAGE_FALL  = 1;
    const int LAST_DAMAGE_HIT   = 2;
 
    const float VALUE_CHECK_INTERVAL        = 1;
    const float SENSITIVTY_PERCENTAGE       = 1;//how much the value needs to change up/down from previous update to trigger a new update(in percent)
 
    PlayerBase m_Player;
    float m_TimeSinceLastTick = VALUE_CHECK_INTERVAL + 1;
    
    string m_CurrentLastDamage;
    
    float m_LastHealthUpdate;
    float m_LastBloodUpdate;
    string m_lastDamageUpdate;
    
    float m_HealthMaxValue;
    float m_BloodMaxValue;
 
    float m_BloodClient;
    float m_HealthClient;
    string m_LastDamageClient;
    
    void DebugMonitorValues(PlayerBase player)
    {
        m_Player = player;
        Init();
