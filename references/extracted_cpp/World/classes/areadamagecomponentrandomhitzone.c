// Source: F:/Games/Dayz/scripts/4_world/classes/areadamage/areadamagenew/damagecomponents/areadamagecomponentrandomhitzone.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class AreaDamageComponentRandomHitzone : AreaDamageComponent
{
    ref array<string>   m_HitZones;
    
    void AreaDamageComponentRandomHitzone(AreaDamageManager parent)
    {
        m_HitZones = new array<string>;
