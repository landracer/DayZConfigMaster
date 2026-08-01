// Source: F:/Games/Dayz/scripts/3_game/dayzanimeventmaps.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//individual sound table consisting of map of parameter hashes as keys and soundbuilder array as values
class SoundLookupTable
{
    void SoundLookupTable()
    {
        m_soundBuilders = new map<int, ref array<SoundObjectBuilder>>();
