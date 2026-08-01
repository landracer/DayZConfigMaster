// Source: F:/Games/Dayz/scripts/5_mission/somemission.c Source File
// Extracted from DayZ Code Explorer by Zeroy

Mission CreateMission(string path)
{
    Print(&quot;Creating Mission: &quot;+ path);
    
    // g_Game.SetMissionPath(path); Done from C++ now
 
    if (g_Game.IsMultiplayer() && g_Game.IsServer())
    {
        return new MissionServer;
    }
 
#ifdef NO_GUI
    return new MissionDummy;
#endif
    MissionMainMenu m;
    if (path.Contains(&quot;NoCutscene&quot;))
    {
        m = new MissionMainMenu();
        m.m_NoCutscene = true;
        return m;
    }
    
    if (path.Contains(&quot;intro&quot;))
    {
        m = new MissionMainMenu();
        m.m_NoCutscene = false;
        return m;
    }
    else
    {
        if( path == &quot;&quot; )
        {
            return new MissionDummy;
        }
#ifndef NO_GUI_INGAME
        return new MissionGameplay;
#else
        return new MissionDummy;
#endif
    }
