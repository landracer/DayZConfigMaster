// Source: F:/Games/Dayz/scripts/4_world/classes/staminamodifierdata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//supplemental data struct for &#39;StaminaModifier&#39; class
class SMDataBase
{
    bool m_InUse = false;
    int m_Type;
    
    void SMDataBase()
    {
        Init();
