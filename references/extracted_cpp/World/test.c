// Source: F:/Games/Dayz/scripts/4_world/entities/vehicles/inheritedcars/test.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// remove by 1.28
 
class CivilianSedan_TEST : CivilianSedan
{
    override void OnDebugSpawn()
    {
        SpawnUniversalParts();
        SpawnAdditionalItems();
        FillUpCarFluids();
 
        GetInventory().CreateInInventory(&quot;CivSedanWheel_TEST&quot;);
        GetInventory().CreateInInventory(&quot;CivSedanWheel_TEST&quot;);
        GetInventory().CreateInInventory(&quot;CivSedanWheel_TEST&quot;);
        GetInventory().CreateInInventory(&quot;CivSedanWheel_TEST&quot;);
 
        GetInventory().CreateInInventory(&quot;CivSedanDoors_Driver&quot;);
        GetInventory().CreateInInventory(&quot;CivSedanDoors_CoDriver&quot;);
        GetInventory().CreateInInventory(&quot;CivSedanDoors_BackLeft&quot;);
        GetInventory().CreateInInventory(&quot;CivSedanDoors_BackRight&quot;);
        GetInventory().CreateInInventory(&quot;CivSedanHood&quot;);
        GetInventory().CreateInInventory(&quot;CivSedanTrunk&quot;);
 
        //-----IN CAR CARGO
        GetInventory().CreateInInventory(&quot;CivSedanWheel_TEST&quot;);
        GetInventory().CreateInInventory(&quot;CivSedanWheel_TEST&quot;);
