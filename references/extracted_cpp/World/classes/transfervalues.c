// Source: F:/Games/Dayz/scripts/4_world/classes/transfervalues.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TransferValues extends Managed
{
    const int TYPE_HEALTH   = 1;
    const int TYPE_BLOOD    = 2;
    
    const float VALUE_CHECK_INTERVAL        = 5;
    const float SENSITIVTY_PERCENTAGE       = 1;//how much the value needs to change up/down from previous update to trigger a new update(in percent)
 
    const int BLOOD_THRESHOLD_LOW = 3000;
    PlayerBase m_Player;
    float m_TimeSinceLastTick = VALUE_CHECK_INTERVAL + 1;
    
    float m_LastHealthUpdate;
    float m_LastBloodUpdate;
    
    //float m_CumulatedHealthDiff;
    //float m_CumulatedBloodDiff;
 
    float m_HealthMaxValue;
    float m_BloodMaxValue;
    
    float m_BloodClient;
    float m_HealthClient;
    
    protected bool m_InitialSyncSent;
    
    void TransferValues(PlayerBase player)
    {
        m_Player = player;
        m_InitialSyncSent = false;
        Init();
