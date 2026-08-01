// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginplayerstatus.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginPlayerStatus extends PluginBase
{
    ref multiMap<int, string>   m_NotifiersLabel;
    ref multiMap<int, int>      m_NotifiersIndexColor;
 
    private ref multiMap<int, string>   m_NotifiersIcons;
 
    void PluginPlayerStatus()
    {
        m_NotifiersLabel = new multiMap<int, string>; // [key] label
        m_NotifiersIndexColor = new multiMap<int, int>; // [key] index, color
 
        m_NotifiersIcons = new multiMap<int, string>; // [key] iconName
        m_NotifiersIcons.Insert( NTFKEY_HUNGRY, &quot;iconHunger&quot; );
        m_NotifiersIcons.Insert( NTFKEY_THIRSTY, &quot;iconThirsty&quot; );
        m_NotifiersIcons.Insert( NTFKEY_HEALTHY, &quot;iconHealth&quot; );
        m_NotifiersIcons.Insert( NTFKEY_BACTERIA, &quot;iconBacteria&quot; );
        m_NotifiersIcons.Insert( NTFKEY_BLEEDISH, &quot;iconBlood&quot; );
        m_NotifiersIcons.Insert( NTFKEY_FEVERISH, &quot;iconTemperature&quot; );
        m_NotifiersIcons.Insert( NTFKEY_FRACTURE, &quot;iconFracture&quot; );
