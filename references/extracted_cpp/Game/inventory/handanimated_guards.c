// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/handanimated_guards.c Source File
// Extracted from DayZ Code Explorer by Zeroy

int SlotToAnimType(notnull Man player, notnull InventoryLocation src, InventoryLocation dst = null)
{
    //Print(&quot;src.GetType() &quot; + src.GetType());
    InventoryLocation invloc1 = new InventoryLocation;
    //InventoryLocation invloc2 = new InventoryLocation;
    
    if ( (dst && dst.GetParent() && !dst.GetParent().GetHierarchyRootPlayer()) || (src && src.GetParent() && !src.GetParent().GetHierarchyRootPlayer()) )
        return -1;
    
    if (dst && (dst.GetType() == InventoryLocationType.ATTACHMENT || dst.GetType() == InventoryLocationType.CARGO))
    {
        invloc1.Copy(dst);
        //invloc2.Copy(src);
    }
    else if (src.GetType() == InventoryLocationType.ATTACHMENT || src.GetType() == InventoryLocationType.CARGO)
    {
        invloc1.Copy(src);
        //invloc2.Copy(dst);
    }
    else
    {
        return -1;
    }
    
    int val = -1;
    if ( invloc1.GetItem() && invloc1.GetItem().GetInventoryHandAnimation(invloc1,val) )
    {
        return val;
    }
    
    if (invloc1.GetType() == InventoryLocationType.ATTACHMENT /*|| src.GetType() == InventoryLocationType.HANDS*/)
    {
        //return WeaponHideShowTypes.HIDESHOW_SLOT_KNIFEBACK;
        switch (invloc1.GetSlot())
        {
            case InventorySlots.SHOULDER:
            {
                if (invloc1.GetItem() && invloc1.GetItem().IsWeapon())
                {
                    return WeaponHideShowTypes.HIDESHOW_SLOT_RFLLEFTBACK;
                }
                else if (invloc1.GetItem() && invloc1.GetItem().IsOneHandedBehaviour())
                {
                    return WeaponHideShowTypes.HIDESHOW_SLOT_1HDLEFTBACK;
                }
                return WeaponHideShowTypes.HIDESHOW_SLOT_2HDLEFTBACK;
            }
            case InventorySlots.MELEE:
            {
                if (invloc1.GetItem() && invloc1.GetItem().IsWeapon())
                {
                    return WeaponHideShowTypes.HIDESHOW_SLOT_RFLRIGHTBACK;
                }
                else if (invloc1.GetItem() && invloc1.GetItem().IsOneHandedBehaviour())
                {
                    return WeaponHideShowTypes.HIDESHOW_SLOT_1HDRIGHTBACK;
                }
                return WeaponHideShowTypes.HIDESHOW_SLOT_2HDRIGHTBACK;
            }
            case InventorySlots.PISTOL:
            {
                EntityAI parent_item = invloc1.GetParent();         // belt
                Man owner;
                if (parent_item)
                    owner = parent_item.GetHierarchyRootPlayer();       // player
                if (!owner)
                    return -1;
                
                EntityAI item1 = owner.GetInventory().FindAttachment(InventorySlots.HIPS);
                EntityAI item2 = parent_item.GetHierarchyParent();
                if (owner && item1 == item2) // is the pistol in a belt holster?
                {
                    return WeaponHideShowTypes.HIDESHOW_SLOT_PISTOLBELT;
                }
                return WeaponHideShowTypes.HIDESHOW_SLOT_PISTOLCHEST;
            }
            case InventorySlots.KNIFE:
                return WeaponHideShowTypes.HIDESHOW_SLOT_KNIFEBACK;
            
            /*case InventorySlots.VEST:
            case InventorySlots.FEET:
            case InventorySlots.BODY:
            case InventorySlots.LEGS:
            case InventorySlots.BACK:
            case InventorySlots.HIPS:
            case InventorySlots.HEADGEAR:
                return WeaponHideShowTypes.HIDESHOW_SLOT_INVENTORY;*/
            
            default:
                return WeaponHideShowTypes.HIDESHOW_SLOT_INVENTORY;
                //Print(&quot;[hndfsm] SlotToAnimType -  not animated slot in src_loc=&quot; + InventoryLocation.DumpToStringNullSafe(invloc1));
        };
        //
        //if (InventorySlots.GetSlotIdFromString(&quot;Pistol&quot;))
    }
    else if (invloc1.GetType() == InventoryLocationType.CARGO)
    {
        EntityAI item = invloc1.GetItem();
        if (item)
        {
            GameInventory itemInventory = item.GetInventory();
            if (itemInventory.HasInventorySlot(InventorySlots.SHOULDER) || itemInventory.HasInventorySlot(InventorySlots.MELEE))
            {
                //Print(&quot;Special inventory anim&quot;);
                if (item.IsWeapon())
                {
                    return WeaponHideShowTypes.HIDESHOW_SLOT_RFLRIGHTBACK;
                }
                else if (item.IsOneHandedBehaviour())
                {
                    return WeaponHideShowTypes.HIDESHOW_SLOT_1HDRIGHTBACK;
                }
                return WeaponHideShowTypes.HIDESHOW_SLOT_2HDRIGHTBACK;
            }
        }
        //Print(&quot;Default inventory anim&quot;);
        return WeaponHideShowTypes.HIDESHOW_SLOT_INVENTORY; //default item animation
    }
    return -1;
