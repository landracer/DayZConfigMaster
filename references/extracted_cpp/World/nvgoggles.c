// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/nvgoggles.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NVGoggles extends PoweredOptic_Base
{
    bool        m_IsLowered;
    Clothing    m_Strap;
    
    void NVGoggles()
    {
        RotateGoggles(true);
