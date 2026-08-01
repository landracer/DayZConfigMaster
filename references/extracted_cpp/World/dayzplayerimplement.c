// Source: F:/Games/Dayz/scripts/4_world/entities/dayzplayerimplement.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*
DayZPlayerImplement
 
this file is implemenation of dayzPlayer in script
- logic of movement
- camera switching logic
 
*/
 
class DayZPlayerCommandDeathCallback : HumanCommandDeathCallback
{
    //----------------------------------
    // callbacks
 
    override void   OnSimulationEnd()
    {
        m_pPlayer.DeathDropHandEntity();
