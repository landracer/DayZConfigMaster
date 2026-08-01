// Source: F:/Games/Dayz/scripts/4_world/classes/remoteplayerdebug/damagedata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DamageData
{
    float m_ValueGlobal;
    float m_ValueBlood;
    float m_ValueShock;
 
    void DamageData(float value_global, float value_blood, float value_shock)
    {
        m_ValueGlobal = value_global;
        m_ValueBlood = value_blood;
        m_ValueShock = value_shock;
