// Source: F:/Games/Dayz/scripts/4_world/static/soundsetmap.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SoundSetMap
{
    ref static map<int, string> m_SoundSetMapIDByID = new map<int, string>;
    ref static map<string, int> m_SoundSetMapIDByName = new map<string, int>;
    
    static void Init()
    {
        string path = &quot;CfgSoundSets&quot;;
        
        int soundCount = g_Game.ConfigGetChildrenCount(path);
        
        for (int i = 1; i < soundCount; i++)
        {
            string soundClassName;
            g_Game.ConfigGetChildName(path, i, soundClassName);
            m_SoundSetMapIDByID.Insert(i,soundClassName);
            m_SoundSetMapIDByName.Insert(soundClassName,i);
            //PrintString(soundClassName);
        }
