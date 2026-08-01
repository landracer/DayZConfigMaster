// Source: F:/Games/Dayz/scripts/4_world/classes/bleedingsources/bleedingsourcesmanagerbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BleedingSourcesManagerBase
{
    ref map<int, ref BleedingSource> m_BleedingSources = new map<int, ref BleedingSource>;
    ref map<string, ref BleedingSourceZone> m_BleedingSourceZone = new map<string, ref BleedingSourceZone>;
    ref map<int, int> m_BleedingSourcesByLocation = new map<int, int>;
    static ref map<int, int> m_BleedingSourcesZonesMaskByLocation = new map<int, int>;//for each inventory location, keep a bitmask where a bit is set to 1 for each bleeding source zone that belongs to that location
    static ref set<int> m_BleedingSourcesLocationsList = new set<int>;
    ItemBase m_Item;//item used to remove the bleeding source
    PlayerBase m_Player;
    //ref map<string, int> m_FireGeomToBit = new map<string, int>;
    ref map<int, string> m_BitToFireGeom = new map<int, string>;
    protected int m_Bit;
    int m_BitOffset = 0;
    
    void BleedingSourcesManagerBase( PlayerBase player )
    {
        m_Player = player;
        Init();
