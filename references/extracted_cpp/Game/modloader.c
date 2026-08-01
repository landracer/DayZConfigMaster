// Source: F:/Games/Dayz/scripts/3_game/client/mods/modloader.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ModLoader
{
    protected static bool                           m_Loaded;
    protected static ref array<ref ModStructure>    m_Mods;
    
    static array<ref ModStructure> GetMods()
    {
        //if( !m_Loaded )
            LoadMods();
        return m_Mods;
