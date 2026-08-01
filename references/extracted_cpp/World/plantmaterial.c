// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/plantmaterial.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlantMaterial extends Inventory_Base
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if (!super.CanPutAsAttachment(parent))
        {
            return false;
        }
        /*const int SLOTS_ARRAY1 = 6;
        const int SLOTS_ARRAY2 = 7;
        bool is_barrel = false;
        bool is_opened_barrel = false;
        bool slot_test1 = true;
        bool slot_test2 = true;
        string slot_names1[SLOTS_ARRAY1] = { &quot;Nails&quot;, &quot;OakBark&quot;, &quot;BirchBark&quot;, &quot;Lime&quot;, &quot;Disinfectant&quot;, &quot;Guts&quot; };
        string slot_names2[SLOTS_ARRAY2] = { &quot;BerryR&quot;, &quot;BerryB&quot;, &quot;Nails&quot;, &quot;OakBark&quot;, &quot;BirchBark&quot;, &quot;Lime&quot;, &quot;Disinfectant&quot; };
 
        
        // is barrel
        if ( parent.IsKindOf(&quot;Barrel_ColorBase&quot;) )
        {
            is_barrel = true;
        }
 
        // is opened barrel             
        if ( is_barrel && parent.GetAnimationPhase(&quot;Lid&quot;) == 1 )
        {
            is_opened_barrel = true;
        }
 
        // all of the barrel attachment slots are empty (first set of ingredients)
        for ( int idx1 = 0; idx1 < SLOTS_ARRAY1 ; idx1++ )
        {
            if ( parent.FindAttachmentBySlotName(slot_names1[idx1]) != NULL )
            {
                slot_test1 = false;
                break;
            }
        }
 
        // all of the barrel attachment slots are empty (second set of ingredients) 
        for ( int idx2 = 0; idx2 < SLOTS_ARRAY2 ; idx2++ )
        {
            if ( parent.FindAttachmentBySlotName(slot_names2[idx2]) != NULL )
            {
                slot_test2 = false;
                break;
            }
        }
        
        if ( ( is_opened_barrel && slot_test1 ) || ( is_opened_barrel && slot_test2) || !is_barrel )
        {
            return true;
        }*/
        
        return false;
