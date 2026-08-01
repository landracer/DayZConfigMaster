// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/basebuildingbase/staticflagpole.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/* 
For use in events/by modders, not part of standard gameplay
Example object spawner json use:
 
{
    &quot;Objects&quot;: [
        {
            &quot;name&quot;: &quot;StaticFlagPole&quot;,
            &quot;pos&quot;: [
                3000,
                57,
                3000
            ],
            &quot;ypr&quot;: [
                0.0,
                0.0,
                0.0
            ],
            &quot;scale&quot;: 1,
            &quot;customString&quot;: &quot;Flag_Bohemia&quot;
        }
     ]
}
 
*/
 
class StaticFlagPole extends TerritoryFlag
{
    override void OnPartDismantledServer( notnull Man player, string part_name, int action_id )
    {
