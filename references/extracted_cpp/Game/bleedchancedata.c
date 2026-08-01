// Source: F:/Games/Dayz/scripts/3_game/bleedchancedata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef map<int,float> BleedChanceMaxMap; //<bloodDamageReceived,chanceMax>

class BleedChanceData : Managed
{
    private static const float BLOOD_HITPOINTS_UNIVERSAL = 100.0;
    
    private static ref map<string, ref BleedChanceMaxMap> m_DamageTypeMap;
    
    static void InitBleedChanceData()
    {
        m_DamageTypeMap = new map<string,ref BleedChanceMaxMap>();
        
        InitMeleeChanceMap();
        InitInfectedChanceMap();
