// Source: F:/Games/Dayz/scripts/3_game/systems/dynamicmusicplayer/dynamicmusicplayerregistry.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DynamicMusicPlayerRegistry
{
    ref map<EDynamicMusicPlayerCategory, ref DynamicMusicPlayerSettings> m_SettingsByCategory;
    
    ref array<ref DynamicMusicTrackData> m_TracksMenu;
    ref array<ref DynamicMusicTrackData> m_TracksCredits;
    ref array<ref DynamicMusicTrackData> m_TracksTime;
    ref array<ref DynamicMusicTrackData> m_TracksLocationStatic;
    ref array<ref DynamicMusicTrackData> m_TracksLocationStaticPrioritized;
    ref array<ref DynamicMusicTrackData> m_TracksLocationDynamic;
    
    protected string m_WorldName = &quot;&quot;;
    
    void DynamicMusicPlayerRegistry()
    {
        PreloadData();
