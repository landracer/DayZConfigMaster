// Source: F:/Games/Dayz/scripts/3_game/entities/dayzinfectedtype.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DayZInfectedAttackType
{
    float m_Distance; // [m]
    int m_Pitch; // 1 = up, 0 = center, -1 = down
    int m_Type; // int argument for attack command
    float m_Subtype; // float argument for attack command
    string m_AmmoType; // ammotype from config
    int m_IsHeavy; // 0 - light attack; 1 - heavy attack;  -> Heavy attack is not blockable
    float m_Cooldown; // [s]
    float m_Probability; // [0..1]
};
 
enum DayZInfectedAttackGroupType
{
    CHASE,
    FIGHT
