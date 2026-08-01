// Source: F:/Games/Dayz/scripts/3_game/analytics/analyticsmanagerserver.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AnalyticsManagerServer
{
    const string STAT_DISTANCE              = &quot;dist&quot;;
    const string STAT_PLAYTIME              = &quot;playtime&quot;;
    const string STAT_PLAYERS_KILLED        = &quot;players_killed&quot;;
    const string STAT_INFECTED_KILLED       = &quot;infected_killed&quot;;
    const string STAT_LONGEST_SURVIVOR_HIT  = &quot;longest_survivor_hit&quot;;
    
    void OnPlayerConnect(Man player)
    {
        player.StatRegister(STAT_DISTANCE);
        player.StatRegister(STAT_PLAYTIME);
        player.StatRegister(STAT_PLAYERS_KILLED);
        player.StatRegister(STAT_INFECTED_KILLED);
        player.StatRegister(STAT_LONGEST_SURVIVOR_HIT);
