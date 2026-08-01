// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/suppressorbase/improvisedsuppressor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ImprovisedSuppressor extends SuppressorBase
{
    
    const int SLOTS_ARRAY = 9;
    
    string slot_names[SLOTS_ARRAY] = { /*&quot;weaponMuzzleAK&quot;, */&quot;weaponBayonetAK&quot;, &quot;weaponBayonet&quot;, &quot;weaponBayonetMosin&quot;, &quot;weaponBayonetSKS&quot;,/* &quot;weaponMuzzleM4&quot;,*/ &quot;weaponMuzzleMosin&quot;, /*&quot;pistolMuzzle&quot;,*/ &quot;weaponMuzzleMP5&quot; };
        
 
    override bool CanPutAsAttachment( EntityAI parent )
    {
        bool cond_state = true;
        if (!super.CanPutAsAttachment(parent)) 
            return false;
        
        for ( int i = 0; i < SLOTS_ARRAY ; i++ )
        {
            if (parent.IsSlotReserved(InventorySlots.GetSlotIdFromString(slot_names[i])))
            {
                cond_state = false;
                break;
            }
            
            if ( parent.FindAttachmentBySlotName(slot_names[i]) != NULL )
            {
                cond_state = false;
                break;
            }
        }
 
        if ( cond_state && !parent.IsKindOf(&quot;PlateCarrierHolster&quot;) && !parent.IsKindOf(&quot;PlateCarrierComplete&quot;) && !parent.IsKindOf(&quot;CarrierHolsterSolo&quot;) && !parent.IsKindOf(&quot;ChestHolster&quot;) )
        {
            return true;
        }
        return false;
