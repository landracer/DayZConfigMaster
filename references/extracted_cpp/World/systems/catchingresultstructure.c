// Source: F:/Games/Dayz/scripts/4_world/systems/animalcatchingsystem/catchingresultstructure.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CatchingResultBase : CatchingResultBasic
{
    override protected float RollChanceSeeded()
    {
        PlayerBase player;
        if (Class.CastTo(player,m_Owner.GetHierarchyRootPlayer()))
            return player.GetRandomGeneratorSyncManager().GetRandom01(RandomGeneratorSyncUsage.RGSAnimalCatching);
        
        ErrorEx(&quot;Player not present or wrong entity used | m_Owner: &quot; + m_Owner + &quot; | root entity: &quot; + m_Owner.GetHierarchyRoot());
        return 2.0;
