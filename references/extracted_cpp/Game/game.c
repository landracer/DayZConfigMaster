// Source: F:/Games/Dayz/scripts/3_game/game.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// #include &quot;Scripts/DayZGame.c&quot;
 
// ---------------------
CGame CreateGame()
{
    //Print(&quot;CreateGame()&quot;);
    g_Game = new DayZGame;
    return g_Game;
