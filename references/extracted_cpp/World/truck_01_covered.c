// Source: F:/Games/Dayz/scripts/4_world/entities/vehicles/inheritedcars/truck_01_covered.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Truck_01_Covered extends Truck_01_Base {};
 
class Truck_01_Covered_Orange extends Truck_01_Covered
{
    override void OnDebugSpawn()
    {
        SpawnUniversalParts();
        SpawnAdditionalItems();
        FillUpCarFluids();
 
        GameInventory inventory = GetInventory();
        inventory.CreateInInventory(&quot;Truck_01_Wheel&quot;);
        inventory.CreateInInventory(&quot;Truck_01_Wheel&quot;);
 
        inventory.CreateInInventory(&quot;Truck_01_WheelDouble&quot;);
        inventory.CreateInInventory(&quot;Truck_01_WheelDouble&quot;);
        inventory.CreateInInventory(&quot;Truck_01_WheelDouble&quot;);
        inventory.CreateInInventory(&quot;Truck_01_WheelDouble&quot;);
 
        inventory.CreateInInventory(&quot;Truck_01_Door_1_1_Orange&quot;);
        inventory.CreateInInventory(&quot;Truck_01_Door_2_1_Orange&quot;);
        inventory.CreateInInventory(&quot;Truck_01_Hood_Orange&quot;);
            
        //-----IN CAR CARGO
        inventory.CreateInInventory(&quot;Truck_01_Wheel&quot;);
        inventory.CreateInInventory(&quot;Truck_01_Wheel&quot;);
        inventory.CreateInInventory(&quot;Truck_01_WheelDouble&quot;);
        inventory.CreateInInventory(&quot;Truck_01_WheelDouble&quot;);
