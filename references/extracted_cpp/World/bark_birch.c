// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/bark_colorbase/bark_birch.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Bark_Birch extends Bark_ColorBase
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if(!super.CanPutAsAttachment(parent)) {return false;}
        const int SLOTS_ARRAY = 8;
        bool is_barrel = false;
        bool is_opened_barrel = false;
        bool slot_test = true;
        string slot_names[SLOTS_ARRAY] = { &quot;BerryR&quot;, &quot;BerryB&quot;, &quot;Plant&quot;, &quot;Nails&quot;, &quot;OakBark&quot;, &quot;Lime&quot;, &quot;Disinfectant&quot;, &quot;Guts&quot; };
 
        
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
 
        // all of the barrel slots are empty
        for ( int i = 0; i < SLOTS_ARRAY ; i++ )
        {
            if ( parent.FindAttachmentBySlotName(slot_names[i]) != NULL )
            {
                slot_test = false;
                break;
            }
        }
        
        if ( ( is_opened_barrel && slot_test ) || !is_barrel )
        {
            return true;
        }
        return false;
